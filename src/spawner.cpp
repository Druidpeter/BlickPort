/*
 * @file spawner.cpp
 * @brief spawner definitions
 *
 */

#include "map.hpp"
#include "entity.hpp"
#include "spawner.hpp"
#include "state.hpp"
#include "clock.hpp"

extern Map map;
extern GsClock gsClock;

/* PRIVATE METHODS */

void Spawner::generateSpawn(sp::SpawnType spawnType, int amount)
{
	// We need to get the bounds of the target viewport, because we
	// don't want to spawn anything anywhere the map is currently
	// visible on screen.

	int viewBounds[4]; = map.target.getNewViewportArray();
	
	int x;
	int y;

	for(int i = amount; i > 0; --i){
	redo:
		map.getTileRand(EMPTY, y, x, 0);

		if(x >= viewBounds[0] && x <= viewBounds[1] &&
		   y >= viewBounds[2] && y <= viewBounds[3]){
			goto redo;
		}

		Spawn sp;
		sp.spawnType = spawnType;
		
		sp.states.emplace_back();
		sp.states.back() = new MapState;
		sp.lookup.insert(std::pair<int, int>(MAP_STATE, sp.states.size()-1));

		// Set the position of the new spawn to the random empty tile
		// we found.
		
		sp.states.back()->position.x = x;
		sp.states.back()->position.y = y;
		
		sp.states.emplace_back();
		sp.states.back() = new SpawnState;
		sp.lookup.insert(std::pair<int, int>(SPAWN_STATE, sp.states.size()-1));

		entities.push(sp);
	}
}

/* PUBLIC METHODS PAST THIS POINT */

void Spawner::spawnPlayer(Spawn *spawn)
{
    MapState *state = static_cast<MapState *>(spawn->getState(MAP_STATE));

    int x;
	int y;
	
    map->getTileRand(EMPTY, y, x, 0);

    state->position.x = x;
    state->position.y = y;

    // Additional state may be set, but for now, we just want position
    // and the control variables.
}

void Spawner::processLevelData(Map *map)
{
	// Here we should receive or somehow acquire the fully generated
	// data from a single level. The spawner needs to take a look at
	// the data and from it calculate what kind of spawns should be
	// generated, to what extent, where to place them, various aspects
	// regardings basic spawn interactions, movements, travel, et al.

	uint16 **dt = map->layout;

	// We sample 15% of the cells in the map.
	int sampleSize = (map->levelHeader.levelWidth *
					  map->levelHeader.levelHeight * 0.15);

	int x;
	int y;

	// We can't actually implement everything discussed below without
	// increasing the sophistication of the map terrain generation
	// first, so this method is just a stub for now. Feel free to read
	// the algo description, though. :D
	
	for(int i = sampleSize; i > 0; --i){
		// map->getTileRand(Y, X);

		// Here's how the complete algorithm *SHOULD* work:

		// We sample a bunch of tiles from the map. For each tile
		// sampled, we check it's tile attributes.

		// Specific types of tile are special. We need to count how
		// many of each type of these special tiles are found, and
		// also the virtual center of mass for each special tile type.

		// Note: virtual center of mass because we *might* at some
		// point want to have different instances of a specific tile
		// type have their own mass and/or density values, but for
		// now, each tile has a mass of simply 1.

		// Hence: The virtual center of mass is really just the
		// average 2D-coordinate location of all tiles of that type.

		// We probably also want to calculate the 2-Dimensional
		// standard deviation, e.g. "spread" from the center of the
		// tiles. Note that this implies taking both the y-dimensional
		// spread as well as the x-dimensional spread separately.

		// Having calculated values for each special type of tile, we
		// then can go about doing the following:

		// For each special tile type, we need to:

		// 1. Generate spawn nodes for different kinds of
		// creatures. These loosely correspond to breeding habitats
		// for various different sorts of creatures.

		// 2. Generate resource distribution fields for various types
		// of resources that are too small or too ubiquitous to be
		// stored at a tile-basis. Stuff that may be found on a
		// specific type of tile at a certain level of threshold
		// probability.

		// Note: The distribution fields are WAY too complex to be
		// stored for each tile. Instead, they need to be calculated
		// as functions in 2 variables, where the variables in
		// questions are x & y coordinates.

		// 3. Generate a weighted region graph tree. This process will
		// involve sampling additional cells, but the basic idea is
		// that this sort of tree functions as a kind of
		// generalization of the more high resolution cell-based map.

		// Essentially, AI logic can use the regiion tree for a higher
		// level of navigation than just finding the shortest route to
		// a specific xy-coordinate point on the 2D map.

		// Rather, using the Region Tree, AI logic can think in terms
		// of needing to find resources such as water, food, areas of
		// the map with various properties, etc. Essentially, the
		// region graph functions as a higher level heuristic for
		// lower level pathfinding algorithms.
	}

	// Of course, all of this is very complex and we need something
	// simple to build upon right now. So for now, just set things up
	// for spawning boars in random locations.

	// We should spawn initial populations of monsters/beasts in this
	// method, though. :D

	spawnRates.insert(std::pair<sp::SMUGBOAR, 1/15>);
	generateSpawn(sp::SMUGBOAR, 15);
}

void Spawner::calculateBiomes()
{
    // Process the current map layout, and generate a graph of
    // habitats corresponding to areas of the map.

    // We do this every time a new map region is loaded in.
}

void Spawner::update()
{
	static int elapsedTime = 0;
	elapsedTime += gsClock.getElapsed();

	int numToSpawn;
	
	for(int i = 0; i < numSpawnable; ++i){
		// So, this algorithm doesn't work at all when we have
		// multiple different species being spawned with different
		// rates. But it should be sufficient when we just have a
		// single type of entity getting spawned in.
		
		numToSpawn = spawnRates[spawns[i]] * elapsedTime;
		generateSpawn(spawns[i], numToSpawn);

		// Reset elapsed time to avoid population explosion. Keep in
		// mind that this doesn't scale for multiple species with
		// different spawn rates, and needs to be changed!!!
		elapsedTime = 0;
	}
}
