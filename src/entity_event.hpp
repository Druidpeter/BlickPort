/**
 * @file entity_event.hpp
 * @brief Events handled by entitie
 *
 */

#ifndef __blickport__entity_event__
#define __blickport__entity_event__

namespace en{
    enum EventType{
        NULL_EVENT
    };

    union EventData{
        int nullData;
    };
};

#endif
