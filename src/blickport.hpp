/*
 * @file blickport.hpp
 * @brief A High Octane Hybrid Beat-em-Up Platformer w/story elements
 *
 * < Blickport project description >
 *
 */


#ifndef __blickport__blickport__
#define __blickport__blickport__

#include <list>

/* GameFlag(s) -- General Operation Modes */

#define GAME_STARTUP 1
#define MAIN_MENU 2
#define NEW_GAME 3
#define LOAD_GAME 4
#define OPTIONS_MENU 5
#define PAUSE_MENU 6
#define GAME_EXIT 7
#define GAME_LEVEL 8
#define STATS_MENU 9

/* Level Flags -- Which level is running */

#define LEVEL_ONE 1
#define LEVEL_TWO 2
#define LEVEL_THREE 3
#define LEVEL_FOUR 4
#define LEVEL_FIVE 5
#define LEVEL_SIX 6

#define BG_XRESOLUTION 1600
#define BG_YRESOLUTION 1000
#define BG_MAGIC_NUMBER_X ((BG_XRESOLUTION/2) - (X_RESOLUTION/2))
#define BG_MAGIC_NUMBER_Y ((BG_YRESOLUTION/2) - (Y_RESOLUTION/2))

#define X_RESOLUTION 600
#define Y_RESOLUTION ((X_RESOLUTION*2)/3)

#define STATE_PROPER 0
#define STATE_IMPROPER 1

#define METHOD_SUCCESS 0
#define METHOD_FAIL -1

namespace gs{
    class Event;
}

class BlickPort{
    int gameFlag; //!< The game re-enters a killed main loop so long
                  //!< as this is nonzero.
    int loopFlag; //!< The main loop runs so long as this is nonzero.
    int gameState; //!< Whether we are handling menus or gameplay.
    int gameLevel; //!< Next Level to load.
    int inputFlag; //!< Are we accepting character at a time input or
                   //!line input?
private:
    void loadNewGame();
    void loadSaveGame();
    void loadLevel(int direction);

    void mainLoop();

    void events();
    void update();
    void render();
private:
    void handleUserInput(int userInput);
public:
    BlickPort();
    ~BlickPort();
    void handleOptions(int argc, char *argv[]);
    void raiseEvent(gs::Event *ev){};
    void raiseEvent(int event);

    int execute();
    void cleanUp();
};
  
#endif
