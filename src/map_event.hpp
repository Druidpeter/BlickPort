/*
 * @file map_event.hpp
 * @brief events and data handled by the map.
 *
 */

#ifndef __blickport__map_event__
#define __blickport__map_event__

struct LevelLink{
    int id;
    int linkType; // 0 -> Biome, 1 -> Interior
};

namespace mp{
    enum EventType{
		GOTO_LEVEL,
    };
	
    union EventData{
		LevelLink nextLevel;
    };

	struct MapEvent{
		EventType eventType;
		EventData eventData;
	};
}

#endif
