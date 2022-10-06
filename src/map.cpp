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
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <curses.h>

#include "blickport.hpp"
#include "map.hpp"
#include "types.hpp"
#include "map_event.hpp"
#include "spawner.hpp"

#include "includes/quadtree/Vector2.h"

extern Map map;
extern Spawner spawner;

/* Private Methods */

// Some interesting thoughts about level generation. Because we have a
// jank and primitive level storage system, all generated levels are
// identified by a strictly ascending integer. That is, each new level
// generated is assigned to it an integer that is always higher than
// the the numbers assigned to all previously generated levels.

// However, we have something called "level mutation
// decay". Essentially, there are multiple overworld algorithms that
// can be used to generate levels. For any specific level generated,
// there is a primary generator, and 0 or more auxiliary
// generators. Further, each generator's "pathology" e.g. how intense
// the effect of that generator is on the level design, is determined
// by a special algorithm:

// First, a level's primary generator is determined by the equivalence
// class to which the integer belongs, out of the left/right cossets
// of zZ+, where 'z' is the number of possible primary generators, and
// 'Z+' is the set of positive integers. I suppose we can also map
// negative integer levels to specialized specific generators for
// specific purposes. (Actually, negative integers would be perfect
// for interior worldspaces, as opposed to overworld levels. In fact,
// I'm now making it so that that is the case. Sweet. :D

// Ignoring interior worldspaces for now, this means that in the case
// of overworld levels, the levels get progressively more and more
// warped the further a player descends through any specific
// "biome". Forests progressively get more "foresty", and so on and so
// forth for other "biomes" as well.

// Actually, it might make more sense to treat consecutively visited
// biomes of the same type to drive the "pathology" attribute. By
// that, I mean tracking a separate pathology value for each biome,
// and then incrementing/decrementing it when passing it through to
// the generators. The logic for incrementing and decrementing is
// simple: Increment the variable if the next level is in the same
// equivalence class as the current one, and decrement the level, if
// the next level is in a different equivalence class. Maybe also have
// greater or lower decrements depending on the specific types of each
// next biome relative to the current one. However, I'm getting ahead
// of myself here.

void Map::generateLevel(int level)
{
	LevelLink nonLink;
	nonLink.id = -1;
	nonLink.linkType = -1;

	generateLevel(level, nonLink);
}

void Map::generateLevel(int level, LevelLink link)
{
	levelHeader.levelType = 0; // Just ignore for now.
	
    // Allocate a completely empty level.	
    uint16_t **dt = new uint16_t*[LEVEL_HEIGHT];

    for(int i = 0; i < LEVEL_HEIGHT; ++i){
        dt[i] = new uint16_t[LEVEL_WIDTH];

        for(int j = 0; j < LEVEL_WIDTH; ++j){
            dt[i][j] = EMPTY;
        }
    }

    // Populate the level using the generation algorithms.
    growthAlgorithm(dt);
    // gableAlgorithm(dt);
    // graphAlgorithm(dt);

    // Add level sign-posts and link points.
    generateLevelLinks(dt, link);
    
    // Free the old level and install the new one.
    if(layout != NULL){
        for(int i = 0; i < LEVEL_HEIGHT; ++i){
            delete [] layout[i];
        } delete [] layout;
    } layout = dt;
}

void Map::generateLevelLinks(uint16_t **data, LevelLink link)
{
    int x = 0;
	int y = 0;

    static std::default_random_engine biomeTypeGenerator;
    static std::uniform_int_distribution<int>
        biomeTypeDistribution(0, NUM_BIOME_TYPES - 1);

    auto biomeDice = std::bind(biomeTypeDistribution,
                               biomeTypeGenerator);

    LevelLink l;
    
    for(int i = 0; i < 2; i++){
		int counter = 0;
		
		while(true){
			x = rand() % LEVEL_WIDTH;
			y = rand() % LEVEL_HEIGHT;

			if((data[y][x] & 1) == 0 ||
			   counter >= 64){
				data[y][x] = SIGN_POST | NOT_WALKABLE;
				break;
			} counter++;
		}
		
		// getEmptyTileLoc(x, y, data);

        l.id = std::abs(currentLevel + 3 + biomeDice() - biomeDice());
        l.linkType = 0;

        std::pair<int, int> tpos(y, x);
        std::pair<std::pair<int, int>, LevelLink> link(tpos, l);
        
        levelLinks.insert(link);
    }

	// We need to add a level link connecting this level with the one
	// it came from. Just brute force copy-paste for now. Refactor
	// eventually. Maybe. It's not really a performance critical part
	// of the code, really.

	// But first, test whether our given level link is valid or not.
	// Sometimes, we have a dummyLink, and hence no levelLink should
	// *actually* be generated.

	if(link.id == -1){
		// No need to create dummy link!
		return;
	}
	
	int counter = 0;
		
	while(true){
		x = rand() % LEVEL_WIDTH;
		y = rand() % LEVEL_HEIGHT;

		if((data[y][x] & 1) == 0 ||
		   counter >= 64){
			data[y][x] = SIGN_POST | NOT_WALKABLE;
			break;
		} counter++;
	}

	std::pair<int, int> tpos(y, x);
	std::pair<std::pair<int, int>, LevelLink> previousLevel(tpos, link);
	levelLinks.insert(previousLevel);
}

//! Growth Algorithm -- Selects semi-random points on the environment
//! to be roots. Then, depending on the surrounding environment,
//! select additional points to become limbs. Next, sample a
//! semi-random number of already selected points to become roots, and
//! for each of those roots, generate new limbs based on the
//! surrounding environment. Repeat until a target number of points
//! has been selected. Mark those nodes as their final terrain type.

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

int Map::loadLevelFromFile(int level)
{
	// I originally thought that just putting the LevelStorage member
	// variable into a void * would handle everything automagically,
	// but it looks like that wasn't the case.

	// So, new plan, flushing storage to file and fetching it back
	// means doing so for each member variable inside Map::storage. We
	// need to take care to dereference and rereference properly the
	// double pointer that is the map layout, because that right there
	// is precisely the point where shit is hitting the fan.

	// Sucks that we have to do it that way, but it is what it is. I'm
	// just glad it was caught early, as opposted to later on down the
	// line. This is what gdb is FOR, people!!
	
    std::string baseDataPath = "./Data/levels/lvl";
	std::string path_lvl = std::to_string(level);
	std::string path_suffix = ".dat";

	std::string filePath = baseDataPath + path_lvl + path_suffix;

	int lvlfd = open(filePath.c_str(), O_RDONLY);

	if(lvlfd == -1){
		// File did not exist! We need to generate it!
		return -1;
	}
	
	LevelStorage tmpStorage;
	
	int numBytesRead = read(lvlfd, &tmpStorage, sizeof(tmpStorage));
	std::cerr << "Have read " << numBytesRead << "bytes.\n";

	levelLinks.clear();

	currentLevel = tmpStorage.levelId;
	levelHeader.levelType = tmpStorage.levelHeader.levelType;
	levelLinks = tmpStorage.levelLinks;
	
	uint16_t **dt = new uint16_t*[tmpStorage.levelHeight];

	for(int i = 0; i < tmpStorage.levelHeight; ++i){
		dt[i] = new uint16_t[tmpStorage.levelWidth];

		for(int j = 0; j < tmpStorage.levelWidth; ++j){
			numBytesRead = read(lvlfd, &(dt[i][j]), sizeof(uint16_t));
		}
	}
	
	if(layout != NULL){
		for(int i = 0; i < LEVEL_HEIGHT; ++i){
			delete [] layout[i];
		} delete [] layout;
	} layout = dt;
	
	close(lvlfd);

	// Just return any value that's not -1.
	return 0;
}

void Map::flushStorage()
{
	std::string baseDataPath = "./Data/levels/lvl";
	std::string path_lvl = std::to_string(storage.levelId);
	std::string path_suffix = ".dat";

	std::string filePath = baseDataPath + path_lvl + path_suffix;

	int lvlfd = open(filePath.c_str(), O_WRONLY | O_CREAT);

	int written = write(lvlfd, &storage, sizeof(storage));

	if(written == -1){
		std::cerr << "Error writing to file: errno = " << errno << " \n";
	} else {
		std::cerr << "Flushed " << written << " bytes to file.\n";		
	}

	for(int i = 0; i < LEVEL_HEIGHT; ++i){
		for(int j = 0; j < LEVEL_WIDTH; ++j){
			written = write(lvlfd, &(layout[i][j]), sizeof(uint16_t));
		}

	}

	if(written == -1){
		std::cerr << "Error writing to file: errno = " << errno << " \n";
	} else {
		std::cerr << "Flushed " << written << " bytes to file.\n";		
	}
	
	close(lvlfd);
}

void Map::traverseLevel(LevelLink link)
{
	LevelStorage *st;

	// Try to load the level from file first. If no file can be found,
	// then we know that we need to generate a new level instead.

	if(loadLevelFromFile(link.id) == -1){
		st = &storage;
		
		st->levelId = currentLevel;
		st->levelHeader = levelHeader;
		st->levelLinks = levelLinks;

		flushStorage();
		
		// Generate level header and
		// set it as member variable.

		levelHeader.levelType = link.linkType;		
		currentLevel = link.id;
		levelLinks.clear();

		LevelLink pl;
		pl.id = st->levelId;
		pl.linkType = st->levelHeader.levelType;
		
		// Generate new level.
		generateLevel(currentLevel, pl);
		
		// Set deepestLevel to levelId.

		deepestLevel = currentLevel;		
	}
}

void Map::dumpCurrentLevel()
{
    // This method is depreciated, and shall be removed.
}

void Map::getLevelFromSignPost(int x, int y, LevelLink *levelLink)
{
	std::pair<int, int> key(y, x);
	
	try{
		LevelLink tmp = levelLinks.at(key);

		levelLink->id = tmp.id;
		levelLink->linkType = tmp.linkType;

		return;
		
	} catch (const std::out_of_range &e){
		std::cerr << "Can't find LevelLink from given Signpost coordinates. How odd.\n";
		exit(EXIT_FAILURE);
	}
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

	int color_attr = 0;
	
    for(int i = starty; i<endy && endy > 0; ++i){
        for(int j = startx; j<endx && endx>0; ++j){
            tile = layout[i][j];

			color_attr = 0;
			
            switch(tile){
            case EMPTY:
                glyph = '.';
				color_attr = 0;
                break;
            case WALL:
                glyph = '#';
				color_attr = 2;
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
                glyph='&';
				color_attr = 3;
                break;
            default:
                glyph = '?';
            }

			if(tile & SIGN_POST){
				glyph = 'T';
				color_attr = 1;
			}

            xdist = j - target.targetX;
            ydist = i - target.targetY;

            xdist = (COLS/2) + xdist;
            ydist = (LINES/2) + ydist;

            if((xdist > 3 && xdist < COLS-4) &&
               (ydist > 2 && ydist < LINES-11)){
                mvaddch(ydist, xdist, glyph | COLOR_PAIR(color_attr));
            }
        }
    }
}


void Map::handleEvent(mp::MapEvent &mapEvent)
{
	mp::MapEvent e = mapEvent;

	switch(e.eventType){
	case mp::GOTO_LEVEL:
		// Note: TraverseLevel is not yet implemented, so instead of
		// spawning the player into a new level, the spawner basically
		// just teleports the player onto a random empty space within
		// the current level. This will be resolved as soon as level
		// traversal is actually implemented.
		
		traverseLevel(e.eventData.nextLevel);
		spawner.spawnPlayer(&player);
		startTracking(&player);
		spawner.processLevelData(&map);
		break;
	default:
		break;
	}
}


// This method ONLY sets up the background environment and level
// layout. entity placement is not handled here.

//! Load a new chunk of the map.

int Map::load(int level)
{
    // Check to see if currentLevel exists as file. If not, save the
    // current level as file.

    std::string tmp = "level";
    tmp.append(std::to_string(currentLevel));

    const char *lvl = tmp.c_str();

    FILE *fd = fopen(lvl, "r");

    if(fd == NULL){
        // File does not exist. Save current level to file.
        dumpCurrentLevel();
    } else {
        fclose(fd);
    } currentLevel = level;


    // Check to see if the next level exists as file. If so, then load
    // it directly. Otherwise, generate it.

    tmp = "level";
    tmp.append(std::to_string(level));

    lvl = tmp.c_str();
    fd = fopen(lvl, "r");

    if(fd == NULL){
        // File does not exist. Generate the level.
        generateLevel(level);
    } else {
        // File does exist. Load level directly from file.
        loadLevelFromFile(level);
        fclose(fd);
    } 
    
    return METHOD_SUCCESS;
}

void Map::setStateData(MapState *state, int spawnType)
{
    // This method is depreciated, and will be removed. The map cage
    // now no longer handles spawning mechanics, for either the player
    // or non-player entities.
}

// If this method is called, it's because some entity has decided
// to attempt to move onto a map location that is not walkable,
// for whatever reason. The map needs to determine the appropriate
// course of action to take when this happens.

// For example: If the entity is the player, and the player has
// decided to move into a signpost, then the map needs to create a
// signpost dialogue menu, set it as the active menu, and trigger
// the dialogue game state. The signpost dialogue menu will either
// return back to normal gamestate if the player cancels, or will
// trigger a new level to load if the player confirms.

// And then the entire game level shifts, and the player along
// with it.

// Alternatively, let's say that an npc or a monster decides to
// move into a signpost. Then the map needs to let the spawner
// know that this has happened, so that the spawner can handle
// moving the npc/monster from one level to the next.

// And all sorts of other things. Like, for example, if the tile
// holds loot. Then the map needs to use the tile location to
// lookup a list/vector/container of (probably item ids) to
// maintain what items exist at that tile location.

// Note: Entities, including the player, can't move onto tiles
// with items on them. They can, however, pickup the items, or
// move the items aside as they pass through.

// Please note, however, that the map is not responsible for doing
// anything with that information. It simply needs to have that
// information on hand in case other entities or cages (AND THEY
// WILL. ) request that data for their own purposes. :D

int Map::collideStep(int x, int y, MapState *state)
{
	uint16_t tile = layout[y][x];

	if(tile & SIGN_POST){
		// Entity has moved into a sign-post.

		// Call Sign_post Prompt:
		// int choice = sign_post_prompt(x, y);

		// Just true for now.
		int choice = true;
		
		// if choice is yes, then travel by signpost. Else, do
		// nothing.

		if(choice){
			mp::MapEvent mapEvent;
			mapEvent.eventType = mp::EventType::GOTO_LEVEL;
			getLevelFromSignPost(x, y, &(mapEvent.eventData.nextLevel));
			handleEvent(mapEvent);
		}
	}

	return 0;
}

// Get a random tile index in the map that matches tile within
// deviance.

void Map::getTileRand(uint16_t tile, int &y, int &x, int deviance)
{
    // Note: Deviance is ignored for now, and may be removed in the
    // future.
    
    int tmpx;
	int tmpy;

    int counter = 0;
    uint16_t tmp;
    
    while(true){
        tmpx = rand() % LEVEL_WIDTH;
        tmpy = rand() % LEVEL_HEIGHT;

        tmp = layout[tmpy][tmpx] & 1;

		// Technically we need to check whether the found layout
		// tile matches the type in the "tile" parameter. But
		// for now, just check to see if it is empty.
		
        if(tmp == 0){
            break;
        }
    };

    y = tmpy;
    x = tmpx;
}

void Map::getEmptyTileLoc(int &y, int &x, uint16_t **data)
{
    int tmpx = 0;
	int tmpy = 0;

    int counter = 0;
    uint16_t **tmp;

	if(layout != NULL){
		tmp = layout;
	} else if(data != NULL){
		tmp = data;
	} else {
		std::cerr << "Can't get empty tile! No level data found. Abort.";
		exit(-1);
	}
}

void Map::startTracking(Spawn *spawn)
{
    tracked = spawn;

    State *tmp_s = tracked->getState(MAP_STATE);
    MapState *ob = dynamic_cast<MapState *>(tmp_s);

    target.targetX = ob->position.x;
    target.targetY = ob->position.y;
}
