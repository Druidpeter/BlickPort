#ifndef __EVENT_HPP__
#define __EVENT_HPP__

// Movement Events recognized by npcs.

#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3

// Events Recognized and handled by all menu objects.

#define CONFIRM_MENU_SELECTION 4
#define TOGGLE_SELECTION_LEFT 5
#define TOGGLE_SELECTION_RIGHT 6
#define MOVE_SELECTION_PREV 7
#define MOVE_SELECTION_NEXT 8

// Events related to changing major game modes.

#define GOTO_MODE_STATS_MENU 10
#define GOTO_MODE_GAME_LEVEL 11
#define GOTO_MODE_PAUSE_MENU 12

namespace gs{
    struct MenuChanged{
        // Flag value determines which menu we are changing to.
        int gameFlag;
    };
    
    struct MenuConfirm{
        // Menu has reached a final state. We need to continue
        // processing based on the selection values of the menu
        // inputs.
        int value; // Just some random var for now.
    };

    enum MenuEventType{
        menuChangedType,
        menuConfirmType
    };

    struct MenuEvent{
        MenuEventType type;
        union data{
            MenuChanged menuChanged;
            MenuConfirm menuConfirm;
        };
    };
    
    enum EventType{
        someEventType,
        MenuEventType
    };
    
    struct Event{
        EventType type;
        union data{
            MenuEvent menuEvent;
        };
    };
}

#endif
