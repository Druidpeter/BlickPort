#include "state.hpp"
#include "entity.hpp"

// Data Cage Includes
#include "map.hpp"
#include "spawner.hpp"
#include "clock.hpp"


extern Map map;
extern GsClock gsClock;

void MapState::initialize(int spawnType)
{
    map.setStateData(this, spawnType);
}

int MapState::update(Spawn *entity)
{
    int x = position.x + velocity.x;
    int y = position.y + velocity.y;
    
    velocity.x = 0;
    velocity.y = 0;

	int mapFeedback = 0;
	
    if((x > -1 && x < LEVEL_WIDTH) &&
       (y > -1 && y < LEVEL_HEIGHT )){
        uint16_t tile = map.getTileAtLoc(x, y); // & 0x0001;

		// NOTE: We really shouldn't handle actual stepping
		// here. Instead of doing a collideStep query if the tile
		// isn't empty, we should ALWAYS just perform a collideStep
		// query for feedback from the map.

		// Of course, having said that, performance might reasonably
		// dictate that we make some allowances for reasonable
		// optimization, so as to avoid overburdening the map
		// cage. Knowing what to do in case of empty tiles might be
		// just such an optimization, but we will eventually have to
		// see. :D
		
        if(tile == EMPTY){
            position.x = x;
            position.y = y;
        } else {
			// Sending the entity pointer to the map is a bad idea. It
			// unnecessarily increases coupling between entities and
			// the map.

			// Instead, we need to fill out a hint structure (the
			// definition of which is in map.hpp, of course) that
			// provides additional information about the client.

			// Of course, for now, just send the this pointer, as
			// that's all we've got set up at the moment. :D
			
			mapFeedback = map.collideStep(x, y, this);
		}

		gsClock.modTime(1);
    }

    // Returning an integer other than false means that the calling
    // logic will delete whatever entity this mapstate is attached
    // to. BE AWARE OF THIS!
    return false;
}

void SpawnState::event(EventId id, EventData data)
{
    switch(id){
    case SPAWN_SET_BASE_STATS:
        setBaseStats(data);
        break;
    default:
        break;
    }
}

void SpawnState::setBaseStats(EventData edata)
{
    int sum = 0;
    
    for(int i = 0; i < NUM_BASE_STATS; i++){
        base[i] = edata.baseStats[i];
        sum += base[i];
    }

    // When base stats are updated, recalculate derived stats.
    derv[MAX_HEALTH] =
        (sum - base[TOUGHNESS]) * (base[TOUGHNESS] + lvl);

    derv[MAX_MANA] =
        (sum - base[AFFINITY]) * (base[AFFINITY] + lvl);

    derv[MAX_WEIGHT] =
        (sum - base[FITNESS]) * (base[FITNESS] + lvl);

    derv[MAX_GELD] =
        (sum - base[CHARISMA]) * (base[CHARISMA] + lvl) * 10;
}
