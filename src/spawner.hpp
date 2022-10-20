/**
 * @file spawner.hpp
 * @brief Handles spawning of entities into the map.
 *
 */

#ifndef __blickport__spawner__
#define __blickport__spawner__

#include <map>

#include "cage.hpp"
#include "state.hpp"

class Map;

// Spawn Rates for different types of entities:

// SMUGBOAR -> 1/15 ticks.

class Spawner : public Cage{
    Map *map;
   
    std::map<int, double> spawnRates;

	// When processing the map, we need to generate a list of
	// spawnable creatures, and store their ids in a freshly allocated
	// 1D array of size numSpawnable.
	int *spawns;

	// For any given map, only a subset of the total number of
	// spawnable entities will actually be spawnable for that map.
	int numSpawnable;
private:
	void generateSpawn(sp::SpawnType spawnType, int amount);
public:
    Spawner(Map *map){
        this->map = map;
    }
        
    virtual ~Spawner(){};

    void spawnPlayer(Spawn *spawn);
	void setSpawnData(SpawnState *state, int spawnType);
	
	
	// We should change this function prototype so that the spawner
	// receives ONLY encapsulated, fully generated, level data,
	// instead of a reference to the map object itself. Better
	// isolation that way.
	
	void processLevelData(Map *map);
    void calculateBiomes();
    
    void event(){};
    virtual void update();
    void render(){};

	void serveClient(SpawnState *state, Spawn *entity);
};

#endif
