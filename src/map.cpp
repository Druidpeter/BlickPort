/*
 * @file map.cpp
 * @brief <brief description>
 *
 */

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "blickport.hpp"
#include "map.hpp"
#include "types.hpp"

#include "includes/quadtree/Vector2.h"

/* Private Methods */

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

void generateTerrain(uint16_t **data)
{
    // Here we have a basic uint16_t **data structure that's given to
    // us, representing the data of the chunk we need to generate.

    // Based off of the value of "currentChunk", and eventually some
    // index values(because let's face it, trying to keep everything
    // derived from just the string value is retarded), we also can
    // instantiate chunk objects and load previously generated chunks
    // from disk. This is so that we can use those chunks as guides
    // for generation later on down the road using wave collapse
    // methods, et al. But for now, we just ignore this functionality,
    // as it's complex enough to implement as it is.

    // We start off by clearing the entire **data to emptiness. Then
    // we call each of the generation algorithms in sequence, "growth,
    // gable, graph" over the data.

    // Each of these algorithms ONLY decides walkability for each cell
    // within **data. Once walkability has been decided, some
    // additional algorithms need to be called.

    // We have one to decide further characteristics of non-walkable
    // tiles, and one for walkable tiles. The various kinds of
    // earth/material around the map are decided by field
    // function. The distance of a particular tile from any specific
    // field sources for a material, combined with proximity to other
    // tiles of similar type, and some mild randomness, will assign
    // what kind of non-walkable or walkable tile it will be.

    // The kind of non-walkable or walkable tile is determined to be
    // will then influence the probabilities for what resources are
    // located around the map. But that will be done MUCH later.
}

// NOTE: The growth algorithm, and all of these algorithms, really, assume that cells inside **data are well-defined, and that the dimensions of **data align to that of CHUNK_WIDTH and CHUNK_HEIGHT.

void growthAlgorithm(uint16_t **data)
{
    // Small optimization, here. ;^D
    const int cw = CHUNK_WIDTH;
    const int ch = CHUNK_HEIGHT;

    int nodesToService = (cw*ch*3)/5;

    std::queue<int> xvals;
    std::queue<int> yvals;

    static std::default_random_engine xgenerator;
    static std::uniform_int_distribution<int> xdistribution(0, cw-1);
    auto xrand = xdistribution(xgenerator);

    static std::default_random_engine ygenerator;
    static std::uniform_int_distribution<int> ydistribution(0, ch-1);
    auto yrand = ydistribution(ygenerator);

    static std::default_random_engine sgenerator;
    static std::uniform_int_distribution<int> sdistribution(-1, 1);
    auto srand = sdistribution(sgenerator);

    
    int x, y;
    uint_8 opcode = 0;
    
    while(nodesToService >= 0){
        if(xvals.size() == 0 && yvals.size() == 0){
        newroot:
            x = xrand(); xvals.push_back(x);
            y = yrand(); yvals.push_back(y);

            --nodesToService;
            continue;
        }

        x = xvals.front();
        y = yvals.front();

        opcode_count = 0;
        
        for(int i = -1; i < 2; ++i){
            for(int j = -1; j < 2; ++j){
                if(i == 0 && j == 0){
                    continue;
                } else if(tileType(data[y+j][x+i]) != WALKABLE){
                    // This "else if" is going to have to change, as
                    // we need to use bitfields for tile data. NOT
                    // just straight up defines. That way lies
                    // madness.
                    
                    opcode += (1 << opcode_count);
                    ++opcode_count;
                }
            }
        }

        int oldx, oldy, s;
        int nodefin = 0;

        for(int i = 0; i < 2; i++){
            // We create 1 or 2 nodes each time.
            
            switch(opcode){
            case 0x00:
                // No surrounding nodes.
                int xc[] = {-1, 1};
                int yc[] = {-1, 1};

                x = xc[rand() % 2];
                y = xc[rand() % 2];

                break;
            case 0x02:
                // 1 neighbor on the left.
                int xc[] = {-2, 1};
                int yc[] = {-1, 1};

                x = xc[rand() % 2];
                y = xc[rand() % 2];

                break;
            case 0x40:
                // 1 neighbor on the right.
                int xc[] = {-1, 2};
                int yc[] = {-1, 1};

                x = xc[rand() % 2];
                y = xc[rand() % 2];

                break;
            case 0x08:
                // 1 neighbor on top.
                int xc[] = {-1, 1};
                int yc[] = {1, -2};

                x = xc[rand() % 2];
                y = xc[rand() % 2];
            
                break;
            case 0x10:
                // 1 neighbor on bottom.
                int xc[] = {-1, 1};
                int yc[] = {-1, 2};

                x = xc[rand() % 2];
                y = xc[rand() % 2];

                break;
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
            case 0x80:
                // 1 neighbor @ SE
                if(rand() %2 == 0){
                    x += -1;
                } else {
                    y += -1;
                } break;
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
            case 0x42:
                // 2 Horizontal Neighbors.
                int xc[] = {-2, 2};
                int yc[] = {-1, 0, 1};

                x += xc[rand() % 2];
                y += yc[rand() % 3];
                break;
            case 0x18:
                // 2 Vertical Neighbors.
                int xc[] = {-1, 0, 1};
                int yc[] = {-2, 2};

                x += xc[rand() % 3];
                y += yc[rand() % 2];
                break;
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
            case 0x81:
                int xc[] = {-2, -2, -1, 2, 2, 1};
                int yc[] = {-1, -2, -2, 1, 2, 2};

                int c = rand() % 6;

                x += xc[c];
                y += yc[c];
                break;
            case 0x24:
                int xc[] = {-2, -2, -1, 2, 2, 1};
                int yc[] = {1, 2, 2, -1, -2, -2};

                int c = rand() % 6;

                x += xc[c];
                y += yc[c];
                break;
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
                data[y][x] = WALL;
            }
            
            xvals.push(x);
            yvals.push(y);

            xvals.pop();
            yvals.pop();

            --nodesToService;

            if((rand() % 2) == true || nodefin == true){
                nodefin = 0;
                break;
            }
        }
    }
}

void loadChunkFromDisk(std::string fd)
{

}

/* Public Methods */

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

//! Load a new chunk of the map.

int Map::loadNextChunk(int x, int y)
{
    // use the two parameters as indexes to modify the currentChunk
    // string.

    // Save the old currentChunk layout to disk.  Check if the new
    // string exists on disk.

    // If the new string exists on disk, then load the chunk from
    // disk. Otherwise, allocate a new layout variable, and
    // generateTerrain on it.

    // Free the data from the old current chunk layout, and swap with
    // the newly allocated layout chunk.

    // Note: for right now, we just generate the terrain of each chunk
    // independently of other chunks. However, we will eventually
    // create a "chunk" class that is capable of loading chunks
    // previously loaded chunks from disk on its own. These objects
    // can then be used by the generate terrain method so as to
    // implement seamless environment generation, using wave collapse
    // or similar.
    
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
