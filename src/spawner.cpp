/*
 * @file spawner.cpp
 * @brief spawner definitions
 *
 */

#include "map.hpp"
#include "entity.hpp"
#include "spawner.hpp"
#include "util.hpp"

void Spawner::spawnPlayer(Spawn *spawn)
{
    gs::MapState *state = static_cast<gs::MapState *>(spawn->getState(MAP_STATE));

    int x, y;
    map->getTileRand(EMPTY, y, x, 0);

    state->position.x = x;
    state->position.y = y;

    // Additional state may be set, but for now, we just want psoition
    // and the control variables.
}
