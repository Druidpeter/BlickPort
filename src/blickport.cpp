/*
 * @file blickport.cpp
 * @brief implementation of top level game routines
 *
 * The entirety of a blickport game instances takes place within a
 * 'blickport' object defined in 'blickport.hpp'. The only thing that
 * main() is responsible for is the creation of a valid blickport game
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
#include <cstdlib>

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

/* Private Methods */
void BlickPort::handleUserInput(int userInput)
{
    loopFlag = false;
    gameFlag = GAME_EXIT;
}

/* Public Methods */

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
    loopFlag = true;

    while(loopFlag == true){
        render();
        events();
        update();
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
        } else if(gsEvent->type == gs::EventType::MenuEventType){
            activeMenu->event(gsEvent);
        }

        // Remove event after having dispatched it.
        eventQueue.pop_front();
    }

    int userInput = getch();
    handleUserInput(userInput);
}        
        
void BlickPort::update()
{
    // Need to swap out the glitchspike map/player/entity logic for
    // blickport. So just return for now.
    return;

    
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
    if(clear() == ERR){
        std::cerr << "Can not clear screen! Killing self.";
        std::exit(ERR);
    }
    
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
    } refresh();
}

//! Blickport game instance constructor

//! Construct a valid, playable game instance of a Blickport
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
    gameFlag = GAME_LEVEL;
    loopFlag = true;
    gameState = STATE_IMPROPER;
    gameLevel = 1;
}

BlickPort::~BlickPort(){}

void BlickPort::handleOptions(int argc, char *argv[])
{
    mainMenu = menuFactory.getMenu("MAIN_MENU");
    optionsMenu = menuFactory.getMenu("OPTIONS_MENU");
    pauseMenu = menuFactory.getMenu("PAUSE_MENU");
}

//! The execute method builds necessary aspects of the game world, and
//! initializes most game-local structures. Whereas the blickport
//! constructor generates a valid blickport instance, the blickport ::
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
        
        mainLoop();
    }
  
    return true;
}

void BlickPort::cleanUp()
{
    delete mainMenu;
    delete optionsMenu;
    delete pauseMenu;

    mainMenu = NULL;
    optionsMenu = NULL;
    pauseMenu = NULL;
}
