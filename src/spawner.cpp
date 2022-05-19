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

    int x, y;
    map->getTileRand(EMPTY, y, x, 0);

    state->position.x = x;
    state->position.y = y;

    // Additional state may be set, but for now, we just want psoition
    // and the control variables.
}
