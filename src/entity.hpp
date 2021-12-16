/**
 * @file entity.hpp
 * @brief Generic game entities
 *
 */

#ifndef __glitchspike__entity__
#define __glitchspike__entity__

#include <vector>
#include <utility>

#include "glitchspike.hpp"
#include "util.hpp"
#include "types.hpp"
#include "event.hpp"

#include "includes/quadtree/Vector2.h"

class Entity{
protected:

public:
    Entity();
    virtual ~Entity();

    virtual void event(gs::Event *event);
    virtual int update();
    virtual void render();
};

struct RenderData{
    int type;
public:


public:
    RenderData(){};
    ~RenderData(){};

    void render();
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

    virtual void event(sf::Event *event);
    virtual void event(gs::Event *event);
    virtual int update();
};

#endif
