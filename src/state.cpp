#include "state.hpp"
#include "entity.hpp"

// Data Cage Includes
#include "map.hpp"
#include "spawner.hpp"


extern Map map;

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

    if((x > -1 && x < LEVEL_WIDTH) &&
       (y > -1 && y < LEVEL_HEIGHT )){
        uint16_t tile = map.getTileAtLoc(x, y); // & 0x0001;

        if(tile == EMPTY){
            position.x = x;
            position.y = y;
        }
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
