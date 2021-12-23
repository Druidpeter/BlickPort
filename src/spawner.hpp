/**
 * @file spawner.hpp
 * @brief Handles spawning of entities into the map.
 *
 */

#ifndef __blickport__spawner__
#define __blickport__spawner__

class Map;

class Spawner{
    Map *map;
public:
    Spawner(Map *map){
        this->map = map;
    }
        
    ~Spawner(){};

    void spawnPlayer(Spawn *spawn);
};

#endif
