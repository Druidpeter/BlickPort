/*
 * @file glitchspike.hpp
 * @brief A High Octane Hybrid Beat-em-Up Platformer w/story elements
 *
 * < Glitchspike project description >
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

/* Level Flags -- Which level is running */

#define STATE_PROPER 0
#define STATE_IMPROPER 1

#define METHOD_SUCCESS 0
#define METHOD_FAIL -1

class BlickPort{
    int gameFlag; //!< The game re-enters a killed main loop so long
                  //!< as this is nonzero.
    int loopFlag; //!< The main loop runs so long as this is nonzero.
    int gameState; //!< Whether we are handling menus or gameplay.
    int gameLevel; //!< Next Level to load.
private:
    void loadNewGame();
    void loadSaveGame();
    void loadLevel();

    void mainLoop();

    void events();
    void update();
    void render();
public:
    BlickPort();
    ~BlickPort();
    void handleOptions(int argc, char *argv[]);
    void raiseEvent(gs::Event *ev){};

    int execute();
    void cleanUp();
};
  
#endif
