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

#include <cstdlib>

#include <curses.h>
#include <time.h>

#include "blickport.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "clock.hpp"
#include "state.hpp"
#include "gui.hpp"
#include "menu.hpp"
#include "spawner.hpp"

BlickPort blickPort;
Player player;

Map map;
Spawner spawner(&map);

GsClock gsClock;

Menu *activeMenu = NULL;
MainMenu mainMenu;
StatsMenu statsMenu;
RaceMenu raceMenu;
ClassMenu classMenu;
Menu optionsMenu;
Menu pauseMenu;

std::list<bp::EventType> eTypeQueue; //!< Game internal events.
std::list<bp::EventData> eDataQueue; //!< Global spawned entities.
std::list<Entity *> entities;


Gui gui; // Head up Display controls.

void init_pairs()
{
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
}

int main(int argc, char *argv[])
{
	std::srand(time(NULL));

    setlocale(LC_ALL, "");
    initscr(); cbreak(); noecho();
	
	start_color();
	init_pairs();

	// std::cerr << "Max Colors: " << COLORS << "\n";
	// std::cerr << "Max Color Pairs: " << COLOR_PAIRS << "\n";
	
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
    
    blickPort.handleOptions(argc, argv);
    blickPort.execute();
    blickPort.cleanUp();

    echo();
    intrflush(stdscr, TRUE);
    
    return 0;
}
