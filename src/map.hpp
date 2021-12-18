/**
 * @file map.hpp
 * @brief The map object and associated classses and structures
 *
 */

#ifndef __blickport__map__
#define __blickportx__map__

#include <cstring>
#include <vector>

#include "entity.hpp"
#include "util.hpp"

#define CHUNK_WIDTH 96
#define CHUNK_HEIGHT 48

#define FLOOR 0
#define WALL 1
#define WATER 2
#define LATCH 3
#define DOOR 4
#define LAVA 5
#define SAND 6
#define GRASS 7
#define MUD 8
#define STONE 9
#define BOULDER 10

#define WALKABLE 11

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

    // Chunk name. Used for generating and loading map chunks from
    // file.
    std::string currentChunk;


    int currentLevel;

    Target target;

    int pSpawnX, pSpawnY;
private:
    void generateTerrain(uint16_t **data);
    void growthAlgorithm();
    void loadChunkFromDisk(std::string fd);
public:
    Map();
    ~Map();

    virtual void event();
    virtual void update();
    virtual void render();
  
    int loadNextChunk(int x, int y);
    int dumpChunk();
    void setStateData(gs::MapState *state, int spawnType);

    // Target really should be turned into a global object. But just
    // get access to it via the map for now.
    Target *getTarget(){ return &target; };

    // Receive a mapState object and check for collisions with the
    // map.
    void collideStep(gs::MapState *state);
};

#endif
