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
#include "util.hpp"
#include "types.hpp"
#include "event.hpp"

#include "includes/quadtree/Vector2.h"

/* Render Data Types */

#define PLAYER 0
#define ENEMY 1

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

class Entity{
protected:

public:
    Entity();
    virtual ~Entity();

    virtual void event(gs::Event *event);
    virtual void event(int event){};
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
protected:
    int spawnType;
    RenderData *data;
    std::vector<gs::State *> states;
    std::map<int, int> lookup;
public:
    Spawn();
    virtual ~Spawn();

    virtual void event(gs::Event *event);
    virtual void event(int event);
    virtual int update();
    virtual void render();

    void setAnimation(int animationType){
        if(data->currAnimType != animationType){
            data->setAnimation(animationType);            
        }
    }

    void initStates(); //!< Special initialization.
    gs::State *getState(int identifier);

    void orientXLeft(){ data->flipX_flag = true; };
    void orientXRight(){ data->flipX_flag = false; };    
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

    virtual void event(gs::Event *event);
    virtual void event(int event);
    virtual int update();
    virtual void render();
};

#endif
