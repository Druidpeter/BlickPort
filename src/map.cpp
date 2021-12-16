/*
 * @file map.cpp
 * @brief <brief description>
 *
 */

#include <iostream>
#include <cmath>
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "blickport.hpp"
#include "map.hpp"
#include "types.hpp"

#include "includes/quadtree/Vector2.h"

Map :: Map()
{

}

Map :: ~Map(){}

void Map :: event()
{

}

void Map :: update()
{

}

void Map :: render()
{    

}

// This method ONLY sets up the background environment and level
// layout. entity placement is not handled here.

int Map::load(int gameLevel)
{

}

void Map::setStateData(gs::MapState *state, int spawnType)
{

}

void Map::collideStep(gs::MapState *state)
{

}
