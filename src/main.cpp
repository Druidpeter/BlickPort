/**
 * @file main.cpp
 * @brief Blickport entry point(s)
 *
 * < Description of Blickport entry point(s)
 *
 */

#include <iostream>

// NOTE: LINES and COLS are defined for us by curses.h, so we don't
// need to perform any gobbledy-gook to get those values. We may need
// to do shit when we start allowing for screen resizing.

// Actually, now that I think about it, we should resize the font to
// match as close as we can specific desired lines and cols values.
// But we'll see.

#include <curses.h>

#include "blickport.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "clock.hpp"
#include "util.hpp"
#include "gui.hpp"
#include "menu.hpp"
#include "event.hpp"

BlickPort blickPort;
Player player;
Map map;
GsClock gsClock;

MenuFactory menuFactory;
Menu *activeMenu = NULL;
Menu *mainMenu = menuFactory.getMenu("MAIN_MENU");
Menu *optionsMenu = menuFactory.getMenu("OPTIONS_MENU");
Menu *pauseMenu = menuFactory.getMenu("PAUSE_MENU");

std::list<gs::Event> eventQueue; //!< Game internal events.
std::list<Entity *> entities; //!< Global spawned entities.

Gui gui; // Head up Display controls.

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    initscr(); cbreak(); noecho();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    
    blickPort.handleOptions(argc, argv);
    blickPort.execute();
    blickPort.cleanUp();

    echo();
    intrflush(stdscr, TRUE);
    
    return 0;
}
