/**
 * @file entity.hpp
 * @brief Generic game entities
 *
 */

#ifndef __blickport__entity__
#define __blickport__entity__

#include <vector>
#include <utility>
#include <map>

#include "blickport.hpp"
#include "entity_event.hpp"
#include "state.hpp"
#include "types.hpp"

#include "includes/quadtree/Vector2.h"

/* Render Data Types */

/* Animation Types */

#define ANIM_IDLE 0
#define ANIM_RUN 1
#define ANIM_JUMP_UP 2
#define ANIM_JUMP_FALL 3
#define ANIM_JUMP_LAND 4

/* Animation Modes */

#define ANIM_NORMAL 0
#define ANIM_FREEZE 1
#define ANIM_LAST_BLIT 2
#define ANIM_NEXT 3

#define MOVE_LEFT 0
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_RIGHT 3

/* Available Races */

#define RACE_HUMAN 0

/* Available Classes */

#define CLASS_NOBLE 0
#define CLASS_TRADESMAN 1
#define CLASS_SOLDIER 2
#define CLASS_DOCTOR 3
#define CLASS_SCHOLAR 4
#define CLASS_PEASANT 5


class Entity{
protected:

public:
    Entity();
    virtual ~Entity();

    virtual void event(en::EventType eType);
    virtual int update();
    virtual void render();
};

struct RenderData{
    int type;
public:

public:
    int fps; //!< Frame rate.
    quadtree::Vector2<int> spriteOrigin; //!< Position on the
                                         //!spritesheet from which to
                                         //!calculate frame offsets.

    std::vector<quadtree::Vector2<int>> currentAnimation;
    std::vector<quadtree::Vector2<int>> offsets;

    int currAnimType;  //!< Whenever we change our animation, we store
                       //!the type of the current animation before the
                       //!switch. This is important.

    int nextAnimType; //! If the render mode for a particular
                      //! animation is ANIM_NEXT, then the animation
                      //! will automatically set itself to whatever
                      //! animation type is set in this variable when
                      //! the current animation ends.

    int numFrames;
    int currentFrame;
    int animationMode;

    //! A bitfield with a bunch of 1 bit flags. Setting the bits can
    //! enable various special behaviors when switching between
    //! frames, wiggling the image about, stretching or shrinking,
    //! putting a single special frame between each frame, blinking on
    //! or off, et al.
    uint8_t statusFlags;
    
    //! Draw the sprite facing left, as opposed to the default facing
    //! right, if this value is true.
    int flipX_flag;
public:
    RenderData(){};
    ~RenderData(){};

    virtual void setAnimation(int animationType);
    
    void render();
};

class Spawn : protected Entity{
    int isMove[2]; //!< Index 0 = movement on x axis. Index 1 =
                   //!< movement on y axis.
    int isJump;

	char glyph;

	friend class Spawner;
protected:
	sp::SpawnType spawnType;
    RenderData *data;
    std::vector<State *> states;
    std::map<int, int> lookup;
public:
    Spawn();
    virtual ~Spawn();

    virtual void event(en::EventType eType);
    virtual int update();
    virtual void render();

    void setAnimation(int animationType){
        if(data->currAnimType != animationType){
            data->setAnimation(animationType);            
        }
    }

    void initStates(); //!< Special initialization.
    State *getState(int identifier);

    void orientXLeft(){ data->flipX_flag = true; };
    void orientXRight(){ data->flipX_flag = false; };

    /* Event Wrappers */
    virtual void setBaseStats(int stats[NUM_BASE_STATS]);
    virtual void setRace(int value);
    virtual void setClass(int value);

	sp::SpawnType getSpawnType(){ return spawnType; };
};

// Different types of spawns, e.g. npcs, enemies, items, need
// different state containers and different cage logic. Therefore,
// SpawnFactory composes each Spawn to have the right states and cage
// interaction values.

class SpawnFactory{
public:
    SpawnFactory();
    Spawn *getSpawn(int spawnType);
};

struct PlayerRenderData : public RenderData{
    // Do be do be do.
public:
    void setAnimation(int animationType);
};

class Player : public Spawn{
    // Stuff go here.
public:
    Player();
    virtual ~Player();

    virtual void event(en::EventType eType);
    virtual void event(int event);
    virtual int update();
    virtual void render();
};

#endif
