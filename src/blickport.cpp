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
#include "menu.hpp"
#include "gui.hpp"
#include "spawner.hpp"

#include "blickport_config.h"

extern Map map;
extern std::list<bp::EventType> eTypeQueue;
extern std::list<bp::EventData> eDataQueue;
extern GsClock gsClock;
extern std::list<Entity *> entities; //!< Global spawned entities.
extern Player player;
extern Gui gui;
extern Spawner spawner;

extern Menu *activeMenu;
extern MainMenu mainMenu;
extern StatsMenu statsMenu;
extern RaceMenu raceMenu;
extern ClassMenu classMenu;
extern Menu optionsMenu;
extern Menu pauseMenu;

/* Private Methods */
void BlickPort::handleUserInput(int userInput)
{
    if(gameState == STATE_PROPER){
        switch(userInput){
        case 'q':
            loopFlag = false;
            gameFlag = GAME_EXIT;
            break;
        case 'h':
            player.event(MOVE_LEFT);
            break;
        case 'j':
            player.event(MOVE_DOWN);
            break;
        case 'k':
            player.event(MOVE_UP);
            break;
        case 'l':
            player.event(MOVE_RIGHT);
            break;
        }
    } else if(gameState == STATE_IMPROPER){
        mn::EventData eventData;
        
        switch(userInput){
        case KEY_ENTER:
        case 'e':
            activeMenu->event(mn::CONFIRM_MENU_SELECTION, eventData);
            break;
        case 'h':
        case KEY_LEFT:
            activeMenu->event(mn::CLEAR_SELECTION, eventData);
            break;
        case 'l':
        case KEY_RIGHT:
            activeMenu->event(mn::TOGGLE_SELECTION, eventData);
            break;
        case 'k':
        case KEY_UP:
            activeMenu->event(mn::SELECT_PREV, eventData);
            break;
        case 'j':
        case KEY_DOWN:
            activeMenu->event(mn::SELECT_NEXT, eventData);
            break;
        default:
            break;
        }
    }
}

/* Public Methods */

void BlickPort::loadNewGame()
{

}

void BlickPort::loadSaveGame()
{

}

//! Setup map and spawn supervisor.

void BlickPort::loadLevel(int direction)
{
    int nextLevel = map.getCurrentLevel() + direction;
    map.load(nextLevel);
        
    // Set starting data for whatever data cages the player(entity)
    // participates in.
    player.initStates();
}

//! Load specific game objects into the global objects queue and gui
//! objects into the gui objects queue. This method calls a helper
//! function based on the flag parameter, where the flag determines
//! which helper, and hence which objects, ultimately do get loaded.

void BlickPort::mainLoop()
{
    loopFlag = true;

    render();
    
    while(loopFlag == true){
        events();
        update();
        render();
    };
}
 
void BlickPort::events()
{
    static bp::EventType eType;
    static bp::EventData eData;
    
    // Service internal game events.
    while(eTypeQueue.size() > 0 && eDataQueue.size() > 0){
        eType = eTypeQueue.front();

        if(eType == bp::EventType::NULL_EVENT){
            // long ass if-else-if chain.
        } else if(eType == bp::EventType::NULL_EVENT){
            // activeMenu->event(gsEvent);
        }

        // Remove event after having dispatched it.
        eTypeQueue.pop_front();
        eDataQueue.pop_front();
    }

    int userInput = getch();
    handleUserInput(userInput);
}        
        
void BlickPort::update()
{
    if(!loopFlag){ return; };
    
    if(gameState == STATE_PROPER){
        player.update();
        map.update();
        spawner.update();
        
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
    if(!loopFlag){ return; };
    
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
    gameFlag = MAIN_MENU;
    loopFlag = true;
    gameState = STATE_IMPROPER;
    gameLevel = 1;
}

BlickPort::~BlickPort(){}

void BlickPort::handleOptions(int argc, char *argv[])
{

}

void BlickPort::raiseEvent(bp::EventType eType, bp::EventData eData)
{
    switch(eType){
    case bp::GOTO_MODE_GAME_LEVEL:
        gameFlag = GAME_LEVEL;
        gameState = STATE_PROPER;
        loopFlag = false;
        break;
    case bp::GOTO_STATS_MENU:
        gameFlag = STATS_MENU;
        gameState = STATE_IMPROPER;
        loopFlag = false;
        break;
    case bp::GOTO_RACE_MENU:
        gameFlag = RACE_MENU;
        gameState = STATE_IMPROPER;
        loopFlag = false;
        break;
    case bp::GOTO_CLASS_MENU:
        gameFlag = CLASS_MENU;
        gameState = STATE_IMPROPER;
        loopFlag = false;
        break;
    case bp::INIT_NEW_GAME:
        break;
    default:
        break;
    }
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
            activeMenu = &mainMenu;
            gameState = STATE_IMPROPER;
            break;
        case STATS_MENU:
            activeMenu = &statsMenu;
            gameState = STATE_IMPROPER;
        case NEW_GAME:
            loadNewGame();
            break;
        case LOAD_GAME:
            loadSaveGame();
            break;
        case OPTIONS_MENU:
            activeMenu = &optionsMenu;
            gameState = STATE_IMPROPER;
            break;
        case PAUSE_MENU:
            activeMenu = &pauseMenu;
            gameState = STATE_IMPROPER;
            break;
        case GAME_LEVEL:
            loadLevel(1);
            spawner.spawnPlayer(&player);
			spawner.processLevelData(&map);
            map.startTracking(&player);
            gameState = STATE_PROPER;
            break;
        case RACE_MENU:
            activeMenu = &raceMenu;
            gameState = STATE_IMPROPER;
            break;
        case CLASS_MENU:
            activeMenu = &classMenu;
            gameState = STATE_IMPROPER;
            break;
        }
        
        mainLoop();
    }
  
    return true;
}

void BlickPort::cleanUp()
{

}
