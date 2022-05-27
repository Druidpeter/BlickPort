/*
 * @file map.cpp
 * @brief <brief description>
 *
 */

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <string>
#include <queue>
#include <functional>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <curses.h>

#include "blickport.hpp"
#include "map.hpp"
#include "types.hpp"

#include "includes/quadtree/Vector2.h"

/* Private Methods */

void Map::generateLevel(int level)
{
    // Allocate a completely empty level.
    
    uint16_t **dt = new uint16_t*[LEVEL_HEIGHT];

    for(int i = 0; i < LEVEL_HEIGHT; ++i){
        dt[i] = new uint16_t[LEVEL_WIDTH];

        for(int j = 0; j < LEVEL_WIDTH; ++j){
            dt[i][j] = EMPTY;
        }
    }

    // Populare the level using the generation algorithms.
    growthAlgorithm(dt);
    // gableAlgorithm(dt);
    // graphAlgorithm(dt);

    // Free the old level and install the new one.
    if(layout != NULL){
        for(int i = 0; i < LEVEL_HEIGHT; ++i){
            delete [] layout[i];
        } delete [] layout;
    } layout = dt;
    
}

//! Growth Algorithm -- Selects semi-random points on the environment
//! to be roots. Then, depending on the surrounding environment,
//! select additional points to become limbs. Next, sample a
//! semi-random number of already selected points to become roots, and
//! for each of those roots, generate new limbs based on the
//! surrounding environment. Repeat a target number of points has been
//! selected. Mark those nodes as their final terrain type.

//! Knights Gable -- Select a semi-random collection of 6 points
//! arranged in a hexagon around a 7th point. Based on the current and
//! surrounding terrain, change the selection of points and mark as
//! their final terrain type. Repeat this process until a target
//! number of gables have been serviced.

//! Graph Charter -- Select a target semi-random collection of points
//! on the environment. Treat these points as vertices of a graph, and
//! set edges based on various graph generation algorithms, such that
//! a spanning graph is formed with a target number of loops. Then,
//! treat the edges as a line segment overlayed on the environment,
//! and mark all points sufficiently near to this line segment to a
//! final terrain type, depending on their distance from the charter.

//! The actual environment generation routine takes into account the
//! terrain data of surrounding environments as an aid to selecting
//! various "anchor points". The anchor points introduce non-random
//! elements into the point selection process, which is crucial for
//! giving each terrain a distinctive feel. :D

void Map::growthAlgorithm(uint16_t **data)
{
    // Small optimization, here. ;^D
    const int cw = LEVEL_WIDTH;
    const int ch = LEVEL_HEIGHT;

    int nodesToService = (cw*ch*2)/5;

    std::queue<int> xvals;
    std::queue<int> yvals;

    static std::default_random_engine xgenerator;
    static std::uniform_int_distribution<int> xdistribution(0, cw-1);
    auto xrand = std::bind(xdistribution, xgenerator);

    static std::default_random_engine ygenerator;
    static std::uniform_int_distribution<int> ydistribution(0, ch-1);
    auto yrand = std::bind(ydistribution, ygenerator);

    static std::default_random_engine sgenerator;
    static std::uniform_int_distribution<int> sdistribution(-1, 1);
    auto srand = std::bind(sdistribution, sgenerator);

    
    int x, y;
    uint8_t opcode = 0;
    
    while(nodesToService >= 0){
        if(xvals.size() == 0 && yvals.size() == 0){
        newroot:
            x = rand() % LEVEL_WIDTH; xvals.push(x);
            y = rand() % LEVEL_HEIGHT; yvals.push(y);

            data[y][x] = data[y][x] | NOT_WALKABLE;

            --nodesToService;
            continue;
        }

        x = xvals.front();
        y = yvals.front();

        int opcode_count = 0;
        uint16_t test = 0;
        
        for(int i = -1; i < 2; ++i){
            for(int j = -1; j < 2; ++j){

                // If cell around node doesn't exist, then treat it as
                // though it were a walkable cell.
                
                if((x+i < 0 || x+i > LEVEL_WIDTH-1) ||
                   (y+j < 0 || y+j > LEVEL_HEIGHT-1 )){
                    ++opcode_count;
                    continue;
                }
                
                test = data[y+j][x+i] & NOT_WALKABLE;
                
                if(i == 0 && j == 0){
                    continue;
                } else if(test == NOT_WALKABLE){
                    opcode += (1 << opcode_count);
                } ++opcode_count;
            }
        }

        int oldx, oldy, s;
        int nodefin = 0;
        
        for(int i = 0; i < 2; i++){
            // We create 1 or 2 nodes each time.
            switch(opcode){
            case 0x00:{
                // No surrounding nodes.
                int xc0[] = {-1, 1};
                int yc0[] = {-1, 1};

                x = xc0[rand() % 2];
                y = xc0[rand() % 2];

            }break;
            case 0x02:{
                // 1 neighbor on the left.
                int xc1[] = {-2, 1};
                int yc1[] = {-1, 1};

                x = xc1[rand() % 2];
                y = xc1[rand() % 2];

            }break;
            case 0x40:{
                // 1 neighbor on the right.
                int xc2[] = {-1, 2};
                int yc2[] = {-1, 1};

                x = xc2[rand() % 2];
                y = xc2[rand() % 2];

            }break;
            case 0x08:{
                // 1 neighbor on top.
                int xc3[] = {-1, 1};
                int yc3[] = {1, -2};

                x = xc3[rand() % 2];
                y = xc3[rand() % 2];
            
            }break;
            case 0x10:{
                // 1 neighbor on bottom.
                int xc4[] = {-1, 1};
                int yc4[] = {-1, 2};

                x = xc4[rand() % 2];
                y = xc4[rand() % 2];

            }break;
            case 0x01:
                // 1 neighbor @ NW
                if(rand() %2 == 0){
                    x += 1;
                } else {
                    y += 1;
                } break;
            case 0x04:
                // 1 neighbor @ SW
                if(rand() %2 == 0){
                    x += 1;
                } else {
                    y += -1;
                } break;
            case 0x20:
                // 1 neighbor @ NE
                if(rand() %2 == 0){
                    x += -1;
                } else {
                    y += 1;
                } break;
            case 0x80:{
                // 1 neighbor @ SE
                if(rand() %2 == 0){
                    x += -1;
                } else {
                    y += -1;
                } }break;
            case 0x12:
                x += -1;
                y += 1;
                nodefin = true;
            case 0x0A:
                x += -1;
                y += -1;
                nodefin = true;
            case 0x48:
                x += 1;
                y += -1;
                nodefin = true;
            case 0x50:
                x += 1;
                y += 1;
            case 0x42:{
                // 2 Horizontal Neighbors.
                int xc5[] = {-2, 2};
                int yc5[] = {-1, 0, 1};

                x += xc5[rand() % 2];
                y += yc5[rand() % 3];
            }break;
            case 0x18:{
                // 2 Vertical Neighbors.
                int xc6[] = {-1, 0, 1};
                int yc6[] = {-2, 2};

                x += xc6[rand() % 3];
                y += yc6[rand() % 2];
            }break;
            case 0x84:
                y += 1;
                nodefin = true;
                break;
            case 0x05:
                x += -1;
                nodefin = true;
                break;
            case 0x21:
                y += -1;
                nodefin = true;
                break;
            case 0xA0:
                x += 1;
                nodefin = true;
                break;
            case 0x81:{
                int xc7[] = {-2, -2, -1, 2, 2, 1};
                int yc7[] = {-1, -2, -2, 1, 2, 2};

                int c = rand() % 6;

                x += xc7[c];
                y += yc7[c];
            }break;
            case 0x24:{
                int xc8[] = {-2, -2, -1, 2, 2, 1};
                int yc8[] = {1, 2, 2, -1, -2, -2};

                int c = rand() % 6;

                x += xc8[c];
                y += yc8[c];
            }break;
            default:
                goto newroot;
            }

            if((x < 0 || x > cw-1) ||
               (y < 0 || y > ch-1)  ){
                goto newroot;
            } else {
                // This statement is going to have to change, as we
                // need to use bitfields for tile data. NOT just
                // straight up defines. That way lies madness.
                data[y][x] = NOT_WALKABLE;
            }
            
            xvals.push(x);
            yvals.push(y);

            xvals.pop();
            yvals.pop();

            --nodesToService;

            if((rand() % 2) == true || nodefin == true){
                nodefin = false;
                break;
            }
        }
    }
}

void Map::gableAlgorithm(uint16_t **data)
{

}

void Map::graphAlgorithm(uint16_t **data)
{

}

void Map::loadLevelFromFile(int level)
{
    std::string datadir = "./Data/level"
    
    std::string tmp = "level";
    tmp.append(std::to_string(level) + ".dat");

    const char *lvl = tmp.c_str();

    FILE *fd = fopen(lvl, "r");
    fseek(fd, 40, SEEK_SET);

    uint16_t **tmp;

    switch(flag){
    case 0:
        tmp = layout;
        break;
    case 1:
        tmp = storage[0].layout;
        break;
    case 2:
        tmp = storage[1].layout;
    }
    
    fread(tmp, sizeof(uint16_t), LEVEL_WIDTH*LEVEL_HEIGHT, fd);
    
    return METHOD_SUCCESS;
}

void Map::flushStorage(int flag)
{
    if(flag == 1){
        // Flush storage[0]
    } else if(flag == -1){
        // Flush storage[1]
    }
}

void Map::traverseLevel(int levelId)
{
    if(levelId > deepestLevel){
        // Generate new level.
    } else if(levelId == currentLevel ||
              levelId == 0){
        // Do nothing.
        return;
    }

    // Load level from storage or file.

    if(levelId == currentLevel+1){
        // Flush storage[0] to file.
        
        // Store currentlevel in storage[0]

        // Restore storage[1] to layout.

        // Fetch from file into storage[1],
        // if possible.
    } else if(levelId == currentLevel-1){
        // Flush storage[1] to file.
        
        // Store currentlevel in storage[1]

        // Restore storage[0] to layout.

        // Fetch from file into storage[0],
        // if possible.
    } else {
        // Fetch levelId from file to layout.

        // Fetch from file into storage[0] and
        // storage[1] levelId-1 and levelId+1,
        // respectively, if possible.
    }
}

void Map::dumpCurrentLevel()
{
    
}

/* Public Methods */

Map :: Map()
{
    layout = NULL;
    currentLevel = 0;

    target.targetX = LEVEL_WIDTH/2;
    target.targetY = LEVEL_HEIGHT/2;
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
    State *tmp_s = tracked->getState(MAP_STATE);
    MapState *ob = dynamic_cast<MapState *>(tmp_s);

    quadtree::Vector2<int> tmpPos = ob->position;
    
    int xDistance = (tmpPos.x - target.targetX);
    int yDistance = (tmpPos.y - target.targetY);

    if(xDistance > 20){
        target.targetX += 1;
    } else if(xDistance < -20){
        target.targetX -= 1;
    }

    if(yDistance > 8){
        target.targetY += 1;
    } else if (yDistance < -8){
        target.targetY -= 1;
    }

    // target.targetX = tmpPos.x;
    // target.targetY = tmpPos.y;
}

void Map :: render()
{
    int viewX = target.targetX - COLS/2;
    int viewW = target.targetX + COLS/2;
    int viewY = target.targetY - LINES/2;
    int viewH = target.targetY + LINES/2;

    int starty = (viewY >= 0) ? viewY: 0;
    int startx = (viewX >= 0) ? viewX: 0;

    int endy = std::min(viewH, LEVEL_HEIGHT);
    int endx = std::min(viewW, LEVEL_WIDTH);

    uint16_t tile;
    char glyph = '.';

    int xdist;
    int ydist;
    
    for(int i = starty; i<endy && endy > 0; ++i){
        for(int j = startx; j<endx && endx>0; ++j){
            tile = layout[i][j];

            switch(tile){
            case EMPTY:
                glyph = '.';
                break;
            case WALL:
                glyph = '#';
                break;
            case DOOR:
                glyph = 'H';
                break;
            case WATER:
                glyph = '~';
                break;
            case STAIR:
                glyph = 'z';
                break;
            case PORTAL:
                glyph = '+';
                break;
            case NOT_WALKABLE:
                glyph='#';
                break;
            default:
                glyph = '?';
            }

            xdist = j - target.targetX;
            ydist = i - target.targetY;

            xdist = (COLS/2) + xdist;
            ydist = (LINES/2) + ydist;

            if((xdist > 3 && xdist < COLS-4) &&
               (ydist > 2 && ydist < LINES-11)){
                mvaddch(ydist, xdist, glyph);
            }
        }
    }
}

// This method ONLY sets up the background environment and level
// layout. entity placement is not handled here.

//! Load a new chunk of the map.

int Map::load(int level, int flag)
{

}

void Map::setStateData(MapState *state, int spawnType)
{
    // This method is depreciated, and will be removed. The map cage
    // now no longer handles spawning mechanics, for either the player
    // or non-player entities.
}

void Map::collideStep(MapState *state)
{

}

// Get a random tile index in the map that matches tile within
// deviance.

void Map::getTileRand(uint16_t tile, int &y, int &x, int deviance)
{
    // Note: Deviance is ignored for now, and may be removed in the
    // future.
    
    int tmpx, tmpy;

    do{
        tmpx = rand() % LEVEL_WIDTH;
        tmpy = rand() % LEVEL_HEIGHT;
    } while(layout[tmpy][tmpx] != tile);

    y = tmpy;
    x = tmpx;
}

void Map::startTracking(Spawn *spawn)
{
    tracked = spawn;

    State *tmp_s = tracked->getState(MAP_STATE);
    MapState *ob = dynamic_cast<MapState *>(tmp_s);

    target.targetX = ob->position.x;
    target.targetY = ob->position.y;
}
