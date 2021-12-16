/**
 * @file map.hpp
 * @brief The map object and associated classses and structures
 *
 */

#ifndef __blickport__map__
#define __blickport__map__

#include <vector>

#include "entity.hpp"
#include "util.hpp"

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

    Target *getTarget(){ return &target; };

    // Receive a mapState object and check for collisions with the
    // map.
    void collideStep(gs::MapState *state);
};

#endif
