/*
 * @file menu.hpp
 *
 */

#ifndef __blickport__menu__
#define __blickport__menu__

#include <vector>
#include "blickport.hpp"

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
    std::string data;
    std::string label;

    int value, min, max;
    int isSelectable;
};

class Menu{
    std::vector<Choice> choiceList;
    int currentChoice;
    
    uint32_t formatCodes;
public:
    Menu(){ currentChoice = -1; };
    ~Menu(){};
  
    void addChoice(std::string data, std::string label, int isSelectable = 1,
                   int value = 0, int min = 0, int max = 1);
    void setCurrentChoice(int index){ currentChoice = index; };

    void event(gs::Event *event);
    void update();
    void render();
};

class MenuFactory{
    void constructMainMenu(Menu *ob, int isUnlocked);
    void constructOptionsMenu(Menu *ob){};
    void constructPauseMenu(Menu *ob){};
public:
    MenuFactory(){}
    Menu *getMenu(std::string choice);
};

#endif
