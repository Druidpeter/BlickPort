#ifndef __blickport__mapspawner__
#define __blickport__mapspawner__

#include "map.hpp"

class Map;

// Receives a reference to a map object. Uses the map data to
// construct "habitat zones" for various animals and game
// factions. Handles population growth, migration, general ecosystem
// behavior for the animals, NPCs, and other non-player entities of
// this world.

class MapSpawner{
    Map *map;
public:
    MapSpawner();
};

#endif
