#include "util.hpp"
#include "map.hpp"
#include "entity.hpp"

extern Map map;

void gs::MapState::initialize(int spawnType)
{
    map.setStateData(this, spawnType);
}

int gs::MapState::update(Spawn *entity)
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
