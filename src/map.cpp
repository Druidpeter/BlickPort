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
    mapWidth = false;
    mapHeight = false;
    
    layout = NULL;

    target.targetX = 0;
    target.targetY = 0;
    target.targetVelX = 0;
    target.targetVelY = 0;

    pSpawnX = 0;
    pSpawnY = 0;

    const double gravity = 8.0;
    target.targetMaxVel = 5.0;
}

Map :: ~Map(){}

void Map :: event()
{

}

void Map :: update()
{
    static const double rubberConstant = 0.2;

    gs::State *tmp_s = player.getState(MAP_STATE);
    gs::MapState *ob = dynamic_cast<gs::MapState *>(tmp_s);

    quadtree::Vector2<int> tmpPos = ob->position;
    quadtree::Vector2<int> tmpVel = ob->velocity;
    
    int xDistance = (tmpPos.x - target.targetX);
    int yDistance = (tmpPos.y - target.targetY);

    target.targetVelX += rubberConstant * (xDistance);
    target.targetVelY += rubberConstant * (yDistance);
    
    if(target.targetVelY > tmpVel.y && tmpVel.y > 0){
        target.targetVelY = tmpVel.y;
    } else if(target.targetVelY < tmpVel.y && tmpVel.y < 0){
        target.targetVelY = tmpVel.y;
    }
    
    if(target.targetVelX > tmpVel.x && tmpVel.x > 0){
        target.targetVelX = tmpVel.x;
    } else if(target.targetVelX < tmpVel.x && tmpVel.x < 0){
        target.targetVelX = tmpVel.x;
    }

    target.targetX += target.targetVelX;
    target.targetY += target.targetVelY;
}

void Map :: render()
{    
    ;
}

// This method ONLY sets up the background environment and level
// layout. entity placement is not handled here.

int Map::load(int gameLevel)
{
    currentLevel = gameLevel;
    std::string levelFd = "./data/";
    
    switch(gameLevel){
    case LEVEL_ONE:
        levelFd += "level1.dat";
        break;
    case LEVEL_TWO:
        levelFd += "level2.dat";
        break;
    case LEVEL_THREE:
        levelFd += "level3.dat";
        break;
    case LEVEL_FOUR:
        levelFd += "level4.dat";
        break;
    case LEVEL_FIVE:
        levelFd += "level5.dat";
        break;
    case LEVEL_SIX:
        levelFd += "level6.dat";
        break;
    default:
        levelFd += "level7.dat";
        break;
    }
    
    int lvlfd = open(levelFd.c_str(), O_RDONLY);

    // Read the first two bytes to determine the map width.
    uint16_t width;
    read(lvlfd, &width, sizeof(uint16_t));

    // Read the second two bytes to determine the map height.
    uint16_t height;
    read(lvlfd, &height, sizeof(uint16_t));

    // Clean up old level if necessary.
    if(layout != NULL && mapWidth > 0 && mapHeight > 0){
        for(int i = 0; i < mapHeight/TILE_HEIGHT; i++){
            delete[] layout[i];
        }

        delete[] layout;
        layout = NULL;
    }

    // (Re-)Allocate level.
    layout = new uint16_t*[height];

    for(int i = 0; i < height; i++){
        layout[i] = new uint16_t[width];
    }

    mapWidth = width * TILE_WIDTH;
    mapHeight = height * TILE_HEIGHT;

    // Read width-size blocks height-size times.
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            read(lvlfd, &layout[i][j], sizeof(uint16_t));
        }
    }
  
    close(lvlfd);

    // The map should actually get the player spawn point from the
    // loaded data file. But for now, just set it manually.
    pSpawnX = TILE_WIDTH * 3;
    pSpawnY = mapHeight - TILE_HEIGHT*8;
    
    return METHOD_SUCCESS;
}

void Map::setStateData(gs::MapState *state, int spawnType)
{
    // These values are pretty much always set.
    
    state->isGravity = true;

    state->velocity.x = 0;
    state->velocity.y = 0;

    state->acceleration.x = 5;
    state->acceleration.y = 5;

    state->hitbox.x = TILE_WIDTH/5;
    state->hitbox.y = TILE_WIDTH/5;

    state->hitDist.x = TILE_WIDTH/5 * 4;
    state->hitDist.y = TILE_WIDTH/5 * 4;
    
    if(spawnType == SPAWN_PLAYER){
        state->position.x = pSpawnX;
        state->position.y = pSpawnY;

        // If the spawnType is that of the player character, then we
        // also need to initialize the target state data at this point as
        // well.
        
        target.targetX = state->position.x;
        target.targetY = state->position.y;
        
        target.targetVelX = state->velocity.x;
        target.targetVelY = state->velocity.y;
    } else if(spawnType != SPAWN_PLAYER){

        // Long ass else-if chain spawning different spawns in the
        // correct locations. Expand this if-else-if as necessary.
    }
}

void Map::collideStep(gs::MapState *state)
{
    int cx;
    int cy;
    
    int xidx;
    int yidx;
    
    if(state->horizontal_axis < 0){
        cx = state->position.x + state->hitbox.x;

        xidx = cx/TILE_WIDTH;
        yidx = (state->position.y + TILE_HEIGHT/3) / TILE_HEIGHT;

        if((layout[yidx][xidx] & 0xF) == FULL_1_0){
            state->position.x += xidx*TILE_WIDTH + TILE_WIDTH - cx;
        }
    } else if(state->horizontal_axis > 0){
        cx = state->position.x + state->hitDist.x;

        xidx = cx/TILE_WIDTH;
        yidx = (state->position.y + TILE_HEIGHT/3) / TILE_HEIGHT;

        if((layout[yidx][xidx] & 0xF) == FULL_1_0){
            state->position.x += xidx*TILE_WIDTH - cx;
        }
    }

    if(state->vertical_axis < 0){
        cy = state->position.y + (state->hitbox.y * 2);

        xidx = (state->position.x + TILE_WIDTH/3) / TILE_WIDTH;
        yidx = cy/TILE_HEIGHT;

        if((layout[yidx][xidx] & 0xF) == FULL_1_0){
            state->position.y += yidx*TILE_HEIGHT + TILE_HEIGHT - cy;
        }
    } else if(state->vertical_axis > 0){
        cy = state->position.y + TILE_HEIGHT;

        xidx = (state->position.x + TILE_WIDTH/3) / TILE_WIDTH;
        yidx = cy/TILE_HEIGHT;

        if((layout[yidx][xidx] & 0xF) == FULL_1_0){
            state->position.y += yidx*TILE_HEIGHT - cy;
        }
    }
}
