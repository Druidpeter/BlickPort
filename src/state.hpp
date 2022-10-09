/*
 * @file state.hpp
 * @brief State Data for various cage systems.
 *
 */

#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <cstdlib>
#include <string>

#include "includes/quadtree/Vector2.h"

#define MAP_STATE 0
#define SPAWN_STATE 1

#define NUM_BASE_STATS 6
#define NUM_DERIVED_STATS 4

class Spawn;

// Note: We lump all events for all states together because
// classes that inherit from State only check for events relevant
// to themselves due to virtual methods, so there's no need for
// the extra layer of separation.
    
enum EventId{
    STATE_NULL_EVENT,
    SPAWN_SET_BASE_STATS
};
    
union EventData{
    // Base Stats
    int baseStats[NUM_BASE_STATS];
};
    
struct State{
    virtual void initialize(int spawnType){};
    virtual int update(){ return false; };
    virtual int update(Spawn *entity){ return false; };
    virtual void event(EventId eventId, EventData data){};
};
    
struct MapState : public State{
    // Base Variables
    quadtree::Vector2<int> baseAccel;
        
    // Control Variables
    int horizontal_axis;
    int vertical_axis;

    // Step Variables
    quadtree::Vector2<int> oldpos;
    quadtree::Vector2<int> hitbox;
    quadtree::Vector2<int> hitDist;
        
    // State Variables
    int isGravity;
        
    quadtree::Vector2<int> position;
    quadtree::Vector2<int> velocity;
    quadtree::Vector2<int> acceleration;
public:
    MapState(){
        horizontal_axis = 0;
        vertical_axis = 0;

        velocity.x = 0;
        velocity.y = 0;
    };
        
    virtual void initialize(int spawnType);
    virtual int update(Spawn *entity);
};

struct SpawnState : public State{
    // Base Stats
    enum {FITNESS, DEXTERITY, TOUGHNESS,
        WIT, CHARISMA, WILLPOWER, AFFINITY};
    int base[NUM_BASE_STATS];

    // Derived Stats
    enum {MAX_HEALTH, MAX_MANA,
        MAX_WEIGHT, MAX_GELD};
    int derv[NUM_DERIVED_STATS];

    // Race Stats
    int raceType;
    int raceMod[NUM_BASE_STATS];

    // Class Stats
    int classType;
    int classMod[NUM_BASE_STATS];

    // Inventory Stats
    // std::list<Equippable> equipment;
    // std::list<Consumable> pouch;
    
    int lvl;
    int xp;

public:
	virtual void initialize(int spawnType){};
	virtual int update(Spawn *entiity){};
	
    void event(EventId id, EventData data);
    void setBaseStats(EventData edata); 
};

#endif
