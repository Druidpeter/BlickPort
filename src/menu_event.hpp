/*
 * @file menu_event.hpp
 * @brief events and data handled by menus.
 *
 */

#ifndef __blickport__menu_event__
#define __blickport__menu_event__

namespace mn{
    enum EventType{
        TOGGLE_SELECTION,
        CLEAR_SELECTION,
        CONFIRM_MENU_SELECTION,
        SELECT_NEXT,
        SELECT_PREV,
        SELECT_NEXT_COL,
        SELECT_PREV_COL
    };
}

#endif
