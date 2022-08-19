/**
 * @file spawner.hpp
 * @brief Handles spawning of entities into the map.
 *
 */

#ifndef __blickport__spawner__
#define __blickport__spawner__

#include <map>

#include "cage.hpp"

class Map;

namespace sp{
    enum SpawnType{
        FIELD_MOUSE
    };
}

class Spawner : public Cage{
    Map *map;

    // 'rate' is the ratio of
    // [(# spawns)/(# ticks)]*10000;
    
    // We do it this way to preserve precision,
    // but this also means that when using the
    // value as a 'ratio' we need to divide the
    // rate by 10,000 first before using it.
    
    std::map<sp::SpawnType, int> spawnRates;
public:
    Spawner(Map *map){
        this->map = map;
    }
        
    virtual ~Spawner(){};

    void spawnPlayer(Spawn *spawn);

	// We should change this function prototype so that the spawner
	// receives ONLY encapsulated, fully generated, level data,
	// instead of a reference to the map object itself. Better
	// isolation that way.
	
	void processLevelData(Map *map);
    void calculateBiomes();
    
    void event(){};
    void update();
    void render(){};
};

#endif
