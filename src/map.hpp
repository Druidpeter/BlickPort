/**
 * @file map.hpp
 * @brief The map object and associated classses and structures
 *
 */

#ifndef __blickport__map__
#define __blickportx__map__

#include <vector>

#include "entity.hpp"
#include "util.hpp"

#define TILE_SIZE 48
#define TILE_WIDTH 48
#define TILE_HEIGHT 48

#define TILE_EMPTY 0

#define FULL_1_0 1
#define FULL_1_1 2
#define FULL_2_1 3
#define HALF_1_0 4
#define HALF_2_1 5
#define FALT_1_0 6
#define FALT_2_1 7
#define QRTR_1_0 8
#define QRTR_1_1 9

#define RAIL_STR8 10
#define RAIL_90DEG 11
#define RAIL_DIAG 12
#define RAIL_JUNCT_T 13
#define RAIL_JUNCT_K 14

#define DOOR 15
#define LEVER 16
#define LADDER 17
#define SPIKES 18

#define NORMAL 0
#define HEAVY 1
#define BOUNCE 2
#define GLASS 3

// ORIENT_RIGHT means vertical rise is visually increasing as we move
// from left to right.

#define ORIENT_RIGHT 0
#define ORIENT_LEFT 1

#define DEG_R0 0
#define DEG_R90 1
#define DEG_R180 2
#define DEG_RN90 3

extern Player player;

struct Target{
    int targetX;
    int targetY;
  
    int targetVelX;
    int targetVelY;

    int targetMaxVel;
};

class Map {
    uint16_t **layout;

    // mapWidth and mapHeight are stored as pixel values.  divide by
    // TILE_HEIGHT and TILE_WIDTH to get num_tiles for the map in each
    // dimension.
    
    int mapWidth;
    int mapHeight;
    int currentLevel;

    Target target;

    int pSpawnX, pSpawnY;
private:

public:
    Map();
    ~Map();

    virtual void event();
    virtual void update();
    virtual void render();
  
    int load(int gameLevel);
    void setStateData(gs::MapState *state, int spawnType);

    // Target really should be turned into a global object. But just
    // get access to it via the map for now.
    Target *getTarget(){ return &target; };

    // Receive a mapState object and check for collisions with the
    // map.
    void collideStep(gs::MapState *state);
};

#endif
