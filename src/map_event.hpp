/*
 * @file map_event.hpp
 * @brief events and data handled by the map.
 *
 */

#ifndef __blickport__map_event__
#define __blickport__map_event__

namespace mp{
    enum EventType{
		GOTO_LEVEL,
    };

	struct GoToLevelData{
		int nextLevel;
	}
	
    union EventData{
		int nextLevel;
        GoToLevelData goToLevelData;
    };

	struct MapEvent{
		EventType eventType;
		EventData eventData;
	}
}

#endif
