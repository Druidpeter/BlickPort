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
    oldpos.x = position.x;
    oldpos.y = position.y;
    
    velocity.x = acceleration.x * horizontal_axis;
    velocity.y = acceleration.y * vertical_axis;

    position.x += velocity.x;
    position.y += velocity.y;

    if(horizontal_axis < 0){
        entity->orientXLeft();
    } else {
        entity->orientXRight();
    }

    if(horizontal_axis == 0 && vertical_axis == 0){
        entity->setAnimation(ANIM_IDLE);
    } else if(horizontal_axis != 0 && vertical_axis == 0){
        entity->setAnimation(ANIM_RUN);
    } else if(vertical_axis < 0){
        entity->setAnimation(ANIM_JUMP_UP);
    } else if(vertical_axis > 0){
        entity->setAnimation(ANIM_JUMP_FALL);
    }

    map.collideStep(this);
    
    // Returning an integer other than false means that the calling
    // logic will delete whatever entity this mapstate is attached
    // to. BE AWARE OF THIS!
    return false;
}
