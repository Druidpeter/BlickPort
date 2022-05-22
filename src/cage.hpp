/**
 *
 * @file cage.hpp
 * @brief This is the cage header file.
 *
 */

#ifndef __blickport__cage__
#define __blickport__cage__

class Cage{
public:
    virtual void event() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
};

#endif
