/**
 * @file clock.hpp
 * @brief Internal world game clock
 *
 */

#ifndef __glitchspike__clock__
#define __glitchspike__clock__

#include "entity.hpp"

class GsClock{
    //
public:
    GsClock();
    virtual ~GsClock();
    virtual void update(){};
};


#endif
