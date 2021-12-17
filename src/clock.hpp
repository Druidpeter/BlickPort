/**
 * @file clock.hpp
 * @brief Internal world game clock
 *
 */

#ifndef __blickport__clock__
#define __blickport__clock__

#include "entity.hpp"

class GsClock{
    //
public:
    GsClock();
    virtual ~GsClock();
    virtual void update(){};
};


#endif
