#include "state.hpp"
#include "entity.hpp"

// Data Cage Includes
#include "map.hpp"
#include "spawner.hpp"
#include "clock.hpp"

extern Map map;
extern Spawner spawner;
extern GsClock gsClock;

void MapState::initialize(int spawnType)
{
    map.setStateData(this, spawnType);
}

int MapState::update(Spawn *entity)
{
	static int bsCounter = 0;
	
	if(entity->spawnType != PLAYER){
		// We're dealing with an NPC, so we don't have controller
		// input to determine velocity. Do it through the dStack.

		// Note: Normally we would implement some A* algorithm or
		// something for path finding, but we actually don't care too
		// much about accurate pathfinding at this state. We just want
		// *some* movement. So setup a counter and a stupidly simple
		// heuristic, and just assume we've reached the goal after 3
		// loops.

		if(!(dStack.empty())){
			std::pair<int, int> dCoord = dStack.back();

			if(dCoord.second - position.x > 0){
				velocity.x = 1;
			} else if(dCoord.second - position.x < 0){
				velocity.x = -1;
			} else {
				velocity.x = 0;
			}

			if(dCoord.first - position.y > 0){
				velocity.y = 1;
			} else if(dCoord.first - position.y < 0){
				velocity.y = -1;
			} else {
				velocity.y = 0;			
			}

			if(bsCounter == 3 &&){
				dStack.pop_back();
				bsCounter = 0;

				velocity.x = 0;
				velocity.y = 0;
			} else {
				++bsCounter;
			}
		}
	}
	
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


void SpawnState::update(Spawn *entity)
{
	
}
