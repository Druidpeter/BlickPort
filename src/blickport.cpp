/*
 * @file rogueboss.cpp
 * @brief implementation of top level game routines
 *
 * The entirety of a rogueboss game instances takes place within a
 * 'rogueboss' object defined in 'rogueboss.hpp'. The only thing that
 * main() is responsible for is the creation of a valid rogueboss game
 * object, and then calling the object's 'execute()' method, which
 * starts the game.
 *
 * Game initialization takes place within the execute() method, at
 * which point control flow is handed over to 'main_loop()' for
 * processing running gameplay, and 'clean_up()' for safely deleting
 * allocated memory after game finish, respectively.
 *
 * The main_loop() additionally calls 5 other helper functions which
 * run in their own threads: 'events()', 'update()', 'render()',
 * 'render_gui()', and 'update_physics()'. These functions check for
 * and handle SFML and internal events, update the state of game
 * objects, render the current state of game objects to the screen,
 * render the current state of gui objects to the screen, and perform
 * physics calculations as necessary, respectively.
 */

#include <iostream>

#include <curses.h>

#include "blickport.hpp"
#include "entity.hpp"
#include "map.hpp"
#include "clock.hpp"
#include "event.hpp"
#include "menu.hpp"
#include "gui.hpp"

extern Map map;
extern std::list<gs::Event> eventQueue;
extern GsClock gsClock;
extern std::list<Entity *> entities; //!< Global spawned entities.
extern Player player;
extern Gui gui;

extern MenuFactory menuFactory;
extern Menu *activeMenu;
extern Menu *mainMenu;
extern Menu *optionsMenu;
extern Menu *pauseMenu;

void BlickPort::loadNewGame()
{

}

void BlickPort::loadSaveGame()
{

}

//! Setup map and spawn supervisor.

void BlickPort::loadLevel()
{
    if(gameLevel < 7 && gameLevel > 0){
        map.load(gameLevel++);
        
        // Set starting data for whatever data cages the player(entity)
        // participates in.
        player.initStates();
    }

}

//! Load specific game objects into the global objects queue and gui
//! objects into the gui objects queue. This method calls a helper
//! function based on the flag parameter, where the flag determines
//! which helper, and hence which objects, ultimately do get loaded.

void BlickPort::mainLoop()
{
    static sf::Time elapsed = sf::seconds(0.0f);
    loopFlag = true;

    while(loopFlag == true){
        events(); // Game waits for input in events.
        update();
        render();
    };
}
 
void BlickPort::events()
{
    static gs::Event *gsEvent;
    
    // Service internal game events.
    while(eventQueue.size() > 0){
        gsEvent = &(eventQueue.front());

        if(gsEvent->type == gs::EventType::someEventType){
            // long ass if-else-if chain.
        } else if(gsEvent->type == gs::EventType::MenuEvent){
            activeMenu.event(gsEvent);
        }

        // Remove event after having dispatched it.
        eventQueue.pop_front();
    }
}        
        
void BlickPort::update()
{
    if(gameState == STATE_PROPER){
        map.update();
        player.update();
        gsClock.update();

        // The update method returning False means remove it
        // from the list.
        
        for(auto it = entities.begin(); it != entities.end(); ++it){
            int op = (*it)->update();
                
            if(op != false){
                it = entities.erase(it);
            }
        }
    } else {
        activeMenu->update();
    }
}

//! Run through the global objects list and call the render() handler
//! for each object on the list. Then return.
 
void BlickPort::render()
{

    // convert this line to ncurses equivalent.
    // screen->clear(sf::Color::Black);
    
    if(gameState == STATE_PROPER){
        // Base Environment
        map.render();

        // Entities
        for(auto it = entities.begin(); it != entities.end(); ++it){
            (*it)->render();
        } player.render();

        // Head-up-Display
        // gui.render();
    } else {
        activeMenu->render();
    } // convert: screen->display();
}

//! BlickPort game instance constructor

//! Construct a valid, playable game instance of a Rogueboss
//! object. The constructor receives options sent in from the
//! commandline environment and uses those options, if any, to guide
//! the instantiation process.

//! Some ideas for options: --god-mode <password>, which allows
//! the player access to an in-game terminal that gives them special
//! powers. This is mostly for testing purposes, but also serves as a
//! nice easter egg. Maybe if the player beats the game under certain
//! conditions, then the god mode password is revealed to them? Food
//! for thought.

BlickPort::BlickPort()
{
    gameFlag = MAIN_MENU;
    loopFlag = true;
    gameState = STATE_IMPROPER;
    gameLevel = 1;
}

BlickPort::~BlickPort(){}

void BlickPort::handleOptions(int argc, char *argv[])
{
    setlocale(LC_ALL, "en_US.utf8");
    initscr(); cbreak(); noecho();
    intrflush(stdscr, FALSE);
    keypad(stdscr, TRUE);
}

//! The execute method builds necessary aspects of the game world, and
//! initializes most game-local structures. Whereas the rogueboss
//! constructor generates a valid rogueboss instance, the rogueboss ::
//! execute() method generates a valid gameplay environment.

int BlickPort::execute()
{
    while(gameFlag != GAME_EXIT){
        switch(gameFlag){
        case MAIN_MENU:
            activeMenu = mainMenu;
            gameState = STATE_IMPROPER;
            break;
        case NEW_GAME:
            loadNewGame();
            break;
        case LOAD_GAME:
            loadSaveGame();
            break;
        case OPTIONS_MENU:
            activeMenu = optionsMenu;
            gameState = STATE_IMPROPER;
            break;
        case PAUSE_MENU:
            activeMenu = pauseMenu;
            gameState = STATE_IMPROPER;
            break;
        case GAME_LEVEL:
            loadLevel();
            gameState = STATE_PROPER;
        }

        // Restart the clock before we enter the main loop. 
        sfClock.restart();

        mainLoop();
    }
  
    return true;
}

void BlickPort::cleanUp()
{
    endwin();

    delete mainMenu;
    delete optionsMenu;
    delete pauseMenu;

    mainMenu = NULL;
    optionsMenu = NULL;
    pauseMenu = NULL;
}
