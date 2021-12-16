/**
 * @file main.cpp
 * @brief Glitchspike entry point(s)
 *
 * < Description of Glitchspike entry point(s)
 *
 */

#include <iostream>

#include <curses.h>

#include "blickport.hpp"
/*
#include "entity.hpp"
#include "map.hpp"
#include "clock.hpp"
#include "util.hpp"
#include "gui.hpp"
#include "menu.hpp"
#include "event.hpp"
*/

BlickPort blickPort;
// Player player;
// Map map;
// GsClock gsClock;
// sf::Clock sfClock;

// MenuFactory menuFactory;
// Menu *activeMenu = NULL;
// Menu *mainMenu = menuFactory.getMenu("MAIN_MENU");
// Menu *optionsMenu = menuFactory.getMenu("OPTIONS_MENU");
// Menu *pauseMenu = menuFactory.getMenu("PAUSE_MENU");

WINDOW *gameScreen;
WINDOW *statsScreen;
WINDOW *logScreen;
WINDOW *menuScreen;

std::list<gs::Event> eventQueue; //!< Game internal events.
std::list<Entity *> entities; //!< Global spawned entities.

Gui gui; // Head up Display controls.

int main(int argc, char *argv[])
{
    blickPort.handleOptions(argc, argv);
    blickPort.execute();
    blickPort.cleanUp();
    
    return 0;
}
