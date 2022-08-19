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
#include "map_event.hpp"

#define LEVEL_WIDTH 200
#define LEVEL_HEIGHT 100

#define LEVEL_ACTIVE_BUFF 0
#define LEVEL_PREV_BUFF 1
#define LEVEL_NEXT_BUFF 2

/* START SPECIAL SECTION */

// Haven't implemented this yet, because it'll break existing
// functionality, but we need a better schema for interpreting tile
// bitstrings, so here it is:

// Map Tile BitString:
// v1: 00 00 0000 0000 0000 LSB (Little Endian)
// v2: 00 00 0000 0000 0000 LSB

// v2 is not used or implemented, but might be in the future.

// "First four bits" refers to the rightmost 4 bits, little endian
// style.

// 1st 4 bits: Codes for basic physics properties regarding movement
// through them. Empty, Occupied, Slow, etc.

// 2nd & 3rd 4 bits: Codes for type of tile, relative to the physics
// properties. So, for example, if the 1st 4 bits specify an empty
// tile, then the 2nd 4 bits determine what kind of floor tile it
// is. e.g. "grass, sand, stone, marble, etc." If, on the other hand,
// the first 4 bits determine "occupied", then the bits code for what
// type of immovable environmental object is occupying the
// tile. e.g. "wall, tree, boulder, ledge, cliff-face, altar, statue,
// furniture, etc."

// 1st 2 of 4th 4 bits: Codes for elevation and visibility values in the form
// of delta values, minimums, maximums assigned to each tile. This
// affects colors and how each tile is rendered, if it is rendered,
// etc.

// 2nd 2 of 4th 4 bits: Codes for non-elevation based visibility
// values. Essentially, things like windows that can be seen out of,
// or crystal walls or doors, periscoes, etc.

/* END SECTION */

// Tile Bitmap of size uint16_t. That is, 16 bits. The following are
// attributes which may be assigned over a single tile, in hexadecimal
// format. They may be OR'ed together to specify a specific tile type.

// These defines code for fundamental properties of the terrain. They
// are involable, and apply to many different tiles on any particular
// map.

#define EMPTY 0x0000
#define WALKABLE 0x0000
#define NOT_WALKABLE 0x0001

#define ENTERABLE 0x000e
#define SIGN_POST (0x0001 << 1)
#define STAIR_DOWN (0x0002 << 1)
#define STAIR_UP (0x0003 << 1)
#define DOOR_PORT (0x0004 << 1)

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

enum BiomeType{
    PLAINS,
    MOUNTAIN,
    DESERT,
    FOREST,
    CAVERN,
    SWAMP,
    TUNDRA,
    RUINS,
    TOWN,
    CITY,
    METROPOLIS,
    NUM_BIOME_TYPES
};

enum InteriorType{
    HUT,
    HOME,
    APARTMENTS,
    MERCHANT,
    SALOON,
    ADMIN,
    MANSION,
    TRAVELING_SHRINE,
    NUM_INTERIOR_TYPES
};

struct BiomeStats{
    int biomePathologies[NUM_BIOME_TYPES];
};

struct LevelLink{
    int id;
    int linkType; // 0 -> Biome, 1 -> Interior
};

struct LevelHeader{
	// Basically, stuff about the level that is useful for
	// generation. Biome type, whether it's an interior or exterior
	// space, general resource and entity hints, et al.

	// Should have done this to begin with instead of trying to be
	// "clever".
};

class Map : public Cage {
    uint16_t **layout;
    LevelStorage storage[2];
    std::map<std::pair<int, int>, LevelLink> levelLinks;

	LevelHeader levelHeader; //<- Should have done this to begin with.
    int currentLevel;
    int deepestLevel;
    Target target;
    Spawn *tracked;

    int pSpawnX, pSpawnY;
private:
    friend class Spawner;
private:
    void generateLevel(int level);
    void generateLevelLinks(uint16_t **data);
    void growthAlgorithm(uint16_t **data);
    void gableAlgorithm(uint16_t **data);
    void graphAlgorithm(uint16_t **data);
    void loadLevelFromFile(int level, int flag);
    void flushStorage(int flag);
    void dumpCurrentLevel();
    void traverseLevel(int levelId);

	void getLevelFromSignPost(int x, int y);
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

	void handleEvent(MapEvent &mapEvent);
  
    int load(int level);
    void setStateData(MapState *state, int spawnType);

    // Target really should be turned into a global object. But just
    // get access to it via the map for now.
    Target *getTarget(){ return &target; };

    // Receive a mapState object and check for collisions with the
    // map.
    int collideStep(int x, int y, MapState *state);

    // Get a random tile index in the map that matches tile within
    // deviance.
    void getTileRand(uint16_t tile, int &y, int &x, int deviance);

    void getEmptyTileLoc(int &y, int &x, uint16_t **data);

    void startTracking(Spawn *spawn);
};

#endif
