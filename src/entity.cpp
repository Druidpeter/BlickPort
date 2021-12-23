#include <cstdlib>
#include <utility>
#include <vector>
#include <array>
#include <iostream>
#include <curses.h>

#include "entity.hpp"
#include "types.hpp"
#include "util.hpp"
#include "map.hpp"

#include "includes/quadtree/Vector2.h"

extern Map map;

Entity::Entity(){}

Entity::~Entity(){}

void Entity::event(gs::Event *event){}

int Entity::update(){ return true; }

void Entity::render(){}

void RenderData::setAnimation(int animationType)
{
    return;
}

void RenderData::render()
{

}

Spawn::Spawn(){}

Spawn::~Spawn(){}

void Spawn::event(gs::Event *event){}

void Spawn::event(int event){}

int Spawn::update(){ return true; }

void Spawn::render()
{

}

void Spawn::initStates()
{
    for(auto it = states.begin(); it != states.end(); ++it){
        (*it)->initialize(this->spawnType);
    }
}

gs::State *Spawn::getState(int identifier)
{
    return states[(lookup.find(identifier)->second)];
}

void PlayerRenderData::setAnimation(int animationType)
{
    currAnimType = animationType;
    
    currentFrame = 0;
    animationMode = 0;
    flipX_flag = 0;

    static int firstRun = true;

    
    if(firstRun == true){
        firstRun = false;
    } else {
        currentAnimation.erase(currentAnimation.begin(), currentAnimation.end());
        offsets.erase(offsets.begin(), offsets.end());
    }

    if(animationType == ANIM_IDLE){
        numFrames = 4;

        currentAnimation.resize(numFrames);
        offsets.resize(numFrames);
        
        for(int i = 0; i < numFrames; ++i){
            currentAnimation[i].x = 48*i;
            currentAnimation[i].y = 48*0;

            offsets[i].x = 48;
            offsets[i].y = 48;
        }
    } else if(animationType == ANIM_RUN){
        numFrames = 10;

        currentAnimation.resize(numFrames);
        offsets.resize(numFrames);
       
        for(int i = 0; i < numFrames; ++i){
            currentAnimation[i].x = 48*i;
            currentAnimation[i].y = 48*1;

            offsets[i].x = 48;
            offsets[i].y = 48;
        }
    } else if(animationType == ANIM_JUMP_UP){
        numFrames = 4;

        currentAnimation.resize(numFrames);
        offsets.resize(numFrames);

        for(int i = 0; i < numFrames; ++i){
            currentAnimation[i].x = 56*i;
            currentAnimation[i].y = 48*2;

            offsets[i].x = 56;
            offsets[i].y = 70;
        } animationMode = ANIM_FREEZE;
    } else if(animationType == ANIM_JUMP_FALL){
        numFrames = 4;

        currentAnimation.resize(numFrames);
        offsets.resize(numFrames);

        for(int i = 0; i < numFrames; ++i){
            currentAnimation[i].x = 56*i + 56*3;
            currentAnimation[i].y = 48*2;

            offsets[i].x = 56;
            offsets[i].y = 70;
        } animationMode = ANIM_LAST_BLIT;       
    } else if(animationType == ANIM_JUMP_LAND){
        numFrames = 3;

        currentAnimation.resize(numFrames);
        offsets.resize(numFrames);

        for(int i = 0; i < numFrames; ++i){
            currentAnimation[i].x = 56*i + 56*5;
            currentAnimation[i].y = 48*2;

            offsets[i].x = 56;
            offsets[i].y = 70;
        } animationMode = ANIM_NEXT;

        nextAnimType = ANIM_IDLE;
    }
}

Player::Player()
{
    spawnType = SPAWN_PLAYER;
    states.emplace_back();
    states.back() = new gs::MapState;
    lookup.insert(std::pair<int,int>(MAP_STATE, states.size()-1));

    data = new PlayerRenderData();
    data->type = PLAYER;
    
    data->fps = 60;
    data->spriteOrigin.x = 0;
    data->spriteOrigin.y = 0;
    
    data->setAnimation(ANIM_IDLE);
}

Player::~Player(){}

void Player::event(gs::Event *event)
{
    
}

// This is for events that require no additional context to serve.

void Player::event(int event)
{
    gs::MapState *state = static_cast<gs::MapState *>(getState(MAP_STATE));
    
    switch(event){
    case MOVE_LEFT:
        state->velocity.x = -1;
        break;
    case MOVE_UP:
        state->velocity.y = -1;
        break;
    case MOVE_DOWN:
        state->velocity.y = 1;
        break;
    case MOVE_RIGHT:
        state->velocity.x = 1;
        break;
    default:
        break;
    }
}

int Player::update()
{
    static int killFlag = false;
    
    for(auto it = states.begin(); it != states.end(); ++it){
        // Basically, any state clients can trigger a kill flag, which
        // will cause the entity to be destroyed after being
        // processed. If kill flag is greater than zero, then multiple
        // kill flag requests were raised, but it doesn't matter
        // because we remove the game entity if at least one is raised
        // at all.
        killFlag += (*it)->update(this);
    }

    return killFlag;
}

void Player::render()
{
    gs::MapState *state = static_cast<gs::MapState *>(getState(MAP_STATE));
    Target *target = map.getTarget();

    int x = state->position.x - target->targetX;
    int y = state->position.y - target->targetY;

    x = COLS/2 + x;
    y = LINES/2 + y;

    mvaddch(y, x, '@');
}
