/*
 * @file menu.hpp
 *
 */

#ifndef __blickport__menu__
#define __blickport__menu__

#include <vector>
#include <cstring>

#include "blickport.hpp"

// Other stuff.

#define NUM_MENU_WIDGETS 10
#define NUM_MENU_CHOICES 10

// Choices for Main Menu
#define CHOICE_NEW_GAME 1
#define CHOICE_CONTINUE 2
#define CHOICE_OPTIONS 3
#define CHOICE_LETTERS 4
#define CHOICE_QUIT 5

// Choices for Options Menu
#define CHOICE_DIFFICULTY 6
#define CHOICE_MUSIC_VOLUME 7
#define CHOICE_SND_FX_VOLUME 8
#define CHOICE_NUM_LIVES 9

#define MENU_SPRITESHEET_FD "../gfx/menuSpriteSheet.png"

#define NO_DELAY_ACTIVATE 0
#define DELAY_ACTIVATE 1

struct Choice{
    std::string label;
    std::string data;

    int value, min, max;

    // If inputField is true, then toggling will either clear or
    // permit the user to input a string value inside 'data' variable.

    // If inputField is false, then toggling will increment or
    // decrement the value variable of the option, not exceeding the
    // minimum or maximum values. Note that standard choice options
    // can be emulated by having a minimum/maximum value pair of
    // [0,1], respectively.
    int inputField;

    int isSelectable;
};

class Menu{
protected:
    std::string menuName;
    std::vector<Choice> choiceList;
    std::vector<int> dividers;
    std::vector<int> selections;

    int nrows;
    int ncols;
    
    int currentChoice;

    // If requireUnique is true, then the menu selection changes by
    // navigating the menu. Toggling selection still works normally.

    // If false, then navigating the menu does not deselect previously
    // selected items, and does not automatically make the currently
    // highlighted node selected. Items must be explicitly selected
    // via toggling. Confirming chosen selections works normally. This
    // is to allow multiple items to be selected at a time within a
    // menu.
    int requireUnique;
private:
    void toggleCurrentChoice(std::string value);
    void moveSelection(int value);
public:
    Menu(){ currentChoice = -1; nrows = 1, ncols = 1; };
    virtual ~Menu(){};
  
    void addChoice(std::string label, int inputField = false, int isSelectable=false,
                   int value = 0, int min = 0, int max = 1);

    void addDivider();
    
    void setCurrentChoice(int index){ currentChoice = index; };

    void setName(const char *name){ menuName = name; };
    char *getName(){
        const char *tmp = menuName.c_str();
        int s = sizeof(tmp)/sizeof(char);

        char *rstr = new char[s];
        std::strcpy(rstr, tmp);
        
        return rstr;
    };

    void setDimensions(int nr, int nc){
        nrows = nr;
        ncols = nc;
    };

    void event(gs::Event *event);
    virtual void event(int event);
    void update();

    virtual void render(int x = 0, int y = 0);
};

class MainMenu : public Menu{
public:
    virtual void render(int x = 0, int y = 0);
};

class StatsMenu : public Menu{
public:
    virtual void render(int x = 0, int y = 0);
};

class MenuFactory{
    void constructMainMenu(Menu *ob);
    void constructStatsMenu(Menu *ob);
    void constructOptionsMenu(Menu *ob){};
    void constructPauseMenu(Menu *ob){};
public:
    MenuFactory(){}
    Menu *getMenu(std::string choice);
};

#endif
