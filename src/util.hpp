#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <cstdlib>
#include <string>

#include "includes/quadtree/Vector2.h"

#define MAP_STATE 0

class Spawn;

namespace gs{
    struct State{
        virtual void initialize(int spawnType){};
        virtual int update(){ return false; };
        virtual int update(Spawn *entity){ return false; };
    };
    
    struct MapState : public State{
        // Base Variables
        quadtree::Vector2<int> baseAccel;
        
        // Control Variables
        int horizontal_axis;
        int vertical_axis;

        // Step Variables
        quadtree::Vector2<int> oldpos;
        quadtree::Vector2<int> hitbox;
        quadtree::Vector2<int> hitDist;
        
        // State Variables
        int isGravity;
        
        quadtree::Vector2<int> position;
        quadtree::Vector2<int> velocity;
        quadtree::Vector2<int> acceleration;
    public:
        MapState(){
            horizontal_axis = 0;
            vertical_axis = 0;

            velocity.x = 0;
            velocity.y = 0;
        };
        
        virtual void initialize(int spawnType);
        virtual int update(Spawn *entity);
    };
}

#endif
