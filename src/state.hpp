/*
 * @file state.hpp
 * @brief State Data for various cage systems.
 *
 */

#ifndef __STATE_HPP__
#define __STATE_HPP__

#include <cstdlib>
#include <string>
#include <list>


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
public:
	virtual ~State(){};
};
    
struct MapState : public State{
	// Need to add more Control Variables:
	
	// We need a stack of locations that represent goal destinations
	// in sequence. The stack may be empty.

	// Note: Technically speaking, we're not operating off of a *true*
	// stack structure. Real stacks are of course always FILO, but
	// real world prioritization algorithms usually can handle edge
	// cases where insertions are done in the middle of the container.

	std::list<std::pair<int, int>> dStack;
	
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

	int doesHaveGoal(){
		// Basically, if our goal stack is empty, return
		// false. Otherwise, return true.

		// Yeah, yeah... I know I can just return !(dStack.empty()),
		// bite my ass.
		
		if(dStack.empty() == true){
			return false;
		} else {
			return true;
		}
	};
};

namespace sp{
    enum SpawnType{
		PLAYER,
        SMUGBOAR,
		NUM_SPAWN_TYPES
    };

	enum ClassType{
		NOBLE,
		TRADESMAN,
		SOLDIER,
		DOCTOR,
		SCHOLAR,
		PEASANT,
		PIDJEER
	};
}

struct SpawnState : public State{	
    // Base Stats
    enum bsType{FITNESS, DEXTERITY, TOUGHNESS,
        WIT, CHARISMA, WILLPOWER, AFFINITY};
    int base[NUM_BASE_STATS];

    // Derived Stats
    enum dsType{MAX_HEALTH, MAX_MANA,
        MAX_WEIGHT, MAX_GELD};
    int derv[NUM_DERIVED_STATS];

    // Race Stats
	sp::SpawnType spawnId;
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
	virtual int update(Spawn *entiity);
	
    void event(EventId id, EventData data);
    void setBaseStats(EventData edata); 
};

struct ItemState : public State{

};

#endif
