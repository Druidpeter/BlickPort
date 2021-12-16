#include <cstdlib>
#include <utility>
#include <vector>
#include <array>
#include <iostream>

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

void Spawn::event(sf::Event *event){}
void Spawn::event(gs::Event *event){}

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

void Player::event(sf::Event *event)
{
    gs::State *tmp_s = states[(lookup.find(MAP_STATE)->second)];
    static gs::MapState *ob = dynamic_cast<gs::MapState *>(tmp_s);

    if(event->type == sf::Event::JoystickMoved){
        int js_val = (event->joystickMove.position /
                                   std::abs(event->joystickMove.position));

        if(std::abs(js_val) < 20){
            js_val = 0;
        }
        
        switch(event->joystickMove.axis){
        case sf::Joystick::X:
            ob->horizontal_axis = js_val;
            break;
        case sf::Joystick::Y:
            ob->vertical_axis = js_val;
        }
    } else if(event->type == sf::Event::KeyPressed){
        switch(event->key.code){
        case sf::Keyboard::Left:
            ob->horizontal_axis += -1;
            break;
        case sf::Keyboard::Right:
            ob->horizontal_axis += 1;
            break;
        case sf::Keyboard::Up:
            ob->vertical_axis += -1;
            break;
        case sf::Keyboard::Down:
            ob->vertical_axis += 1;
        }
    } else if(event->type == sf::Event::KeyReleased){
        switch(event->key.code){
        case sf::Keyboard::Left:
            ob->horizontal_axis -= -1;
            break;
        case sf::Keyboard::Right:
            ob->horizontal_axis -= 1;
            break;
        case sf::Keyboard::Up:
            ob->vertical_axis -= -1;
            break;
        case sf::Keyboard::Down:
            ob->vertical_axis -= 1;
        }
    }
}

void Player::event(gs::Event *event)
{
    
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
