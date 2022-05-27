/**
 * @file map.hpp
 * @brief The map object and associated classses and structures
 *
 */

#ifndef __blickport__map__
#define __blickport__map__

#include <cstring>
#include <vector>

#include <stdio.h>

#include "entity.hpp"
#include "state.hpp"
#include "cage.hpp"

#define LEVEL_WIDTH 200
#define LEVEL_HEIGHT 100

// Tile Bitmap of size uint16_t. That is, 16 bits. The following are
// attributes which may be assigned over a single tile, in hexadecimal
// format. They may be OR'ed together to specify a specific tile type.

// These defines code for fundamental properties of the terrain. They
// are involable, and apply to many different tiles on any particular
// map.

#define EMPTY 0x0000
#define WALKABLE 0x0000
#define NOT_WALKABLE 0x0001
#define ENTERABLE 0x0002
#define FLUIDSPACE 0x0004
#define SEETHROUGH 0x0008

// These defines code for a crude elevation mechanic of the map. Each
// tile is assigned a delta value, which is equivalent to the rate of
// change in the x and y dimension at each tile point.

// Elevation values affect visibility and awareness gameplay.

#define X_DELTA_NEG 0x1000
#define Y_DELTA_NEG 0x2000
#define LOCAL_MIN 0x4000
#define LOCAL_MAX 0x8000
#define SADDLE_PT 0x0800

// NOTE: We have 128 possible unique tyle types to potentially work
// with. These tiles all have specific gameplay properties that apply
// to only tiles of their specific type. If, in the future, we end up
// needing more than 128 tile types, we can simply add another
// uint16_t data type to the map layout data.

#define FLOOR 0x0010
#define WALL (0x0020 | NOT_WALKABLE)
#define DOOR (0x0030 | ENTERABLE)
#define WATER (0x0040 | FLUIDSPACE)
#define STAIR (0x0050 | ENTERABLE)
#define PORTAL (0x0060 | ENTERABLE)

extern Player player;

struct Target{
    int targetX;
    int targetY;
  
    int targetVelX;
    int targetVelY;

    int targetMaxVel;
};

struct LevelStorage{
    int levelId;
    uint16_t **layout;
};

class Map : public Cage {
    uint16_t **layout;
    LevelStorage storage[2];
    
    int currentLevel;
    int deepestLevel;
    Target target;
    Spawn *tracked;

    int pSpawnX, pSpawnY;
private:
    friend class Spawner;
private:
    void generateLevel(int level);
    void growthAlgorithm(uint16_t **data);
    void gableAlgorithm(uint16_t **data);
    void graphAlgorithm(uint16_t **data);
    void loadLevelFromFile(int level);
    void flushStorage(int flag);
    void dumpCurrentLevel();
    void traverseLevel(int levelId);
public:
    // Getters and Setters
    int getCurrentLevel(){return currentLevel; }
    uint16_t getTileAtLoc(int x, int y){
        return layout[y][x];
    }
public:
    Map();
    ~Map();

    virtual void event();
    virtual void update();
    virtual void render();
  
    int load(int level, int flag);
    void setStateData(MapState *state, int spawnType);

    // Target really should be turned into a global object. But just
    // get access to it via the map for now.
    Target *getTarget(){ return &target; };

    // Receive a mapState object and check for collisions with the
    // map.
    void collideStep(MapState *state);

    // Get a random tile index in the map that matches tile within
    // deviance.
    void getTileRand(uint16_t tile, int &y, int &x, int deviance);

    void startTracking(Spawn *spawn);
};

#endif
