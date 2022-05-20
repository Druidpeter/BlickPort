/*
 * @file bp_event.hpp
 * @brief All internal game events defined in this file.
 *
 * We declare both an enumeration and a union structure holding data
 * structures relevant to each enumeration value. Our internal
 * blickPort.raiseEvent() method takes these and does stuff with them
 * when called.
 *
 * Almost anything can raise a blickPort event at any
 * time. Responsibility is on the calling method (and therefore, the
 * programmers!) to make sure a correct enumeration and data structure
 * combination are used as parameters.
 */

#ifndef __blickport__bp_event__
#define __blickport__bp_event__

namespace bp{
    enum EventType{
        NULL_EVENT,
        GOTO_MODE_GAME_LEVEL,
        INIT_NEW_GAME,
        GOTO_STATS_MENU,
        GOTO_RACE_MENU,
        GOTO_CLASS_MENU,
    };
    
    union EventData{
        int value; 
    };
}

#endif
