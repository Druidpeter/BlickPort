/*
 * @file spawner.cpp
 * @brief spawner definitions
 *
 */

#include "map.hpp"
#include "entity.hpp"
#include "spawner.hpp"
#include "state.hpp"

void Spawner::spawnPlayer(Spawn *spawn)
{
    MapState *state = static_cast<MapState *>(spawn->getState(MAP_STATE));

    int x;
	int y;
	
    map->getTileRand(EMPTY, y, x, 0);

    state->position.x = x;
    state->position.y = y;

    // Additional state may be set, but for now, we just want psoition
    // and the control variables.
}

void Spawner::processLevelData(Map *map)
{
	// Here we should receive or somehow acquire the fully generated
	// data from a single level. The spawner needs to take a look at
	// the data and from it calculate what kind of spawns should be
	// generated, to what extent, where to place them, various aspects
	// regardings basic spawn interactions, movements, travel, et al.
}

void Spawner::calculateBiomes()
{
    // Process the current map layout, and generate a graph of
    // habitats corresponding to areas of the map.

    // We do this every time a new map region is loaded in.
}

void Spawner::update()
{
    // Note: The following algorithm works well for birth rates, but
    // we will eventually need a much better system that handles:

    // 1. Birth rates, Death Rates, Entering map area from different
    // map regions of the world. (e.g. from outside to inside
    // different worldspaces)

    // 2. Spawning mechanics for corpse remains, as well as item
    // drops, and other non-living things. All of this needs to be
    // handled by the spawner.

    // * * *
    
    // Get elapsed time from GsClock object.
    
    // Run through list of spawnables. See if any spawnable types'
    // timelength trigger has passed.

    // If so, spawn required number of that type into the map near a
    // calculated spawnpoint.

    // Note: When calculating where to spawn a creature, the following
    // precedence order is used:

    // 1. Pick a random female inside habitate.
    // 2. Pick a random female.
    // 3. Pick a random point on map, strongly weighted towards habitat.   
}
