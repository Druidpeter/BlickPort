#ifndef __EVENT_HPP__
#define __EVENT_HPP__

namespace gs{
    struct MenuChanged{
        // Flag value determines which menu we are changing to.
        int gameFlag;
    };
    
    struct MenuConfirm{
        // Menu has reached a final state. We need to continue
        // processing based on the selection values of the menu
        // inputs.
        int bool; // Just some random var for now.
    };

    enum MenuEventType{
        menuChangedType,
        menuConfirmType
    };

    struct MenuEvent{
        MenuEventType type;
        union data{
            MenuChanged;
            MenuConfirm;
        };
    };
    
    enum EventType{
        someEventType,
        MenuEventType
    };
    
    struct Event{
        EventType type;
        union data{
            MenuEvent;
        };
        
    };
}

#endif
