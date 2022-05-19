/*
 * @file menu.hpp
 *
 */

#ifndef __blickport__menu__
#define __blickport__menu__

#include <vector>
#include <cstring>

#include "blickport.hpp"
#include "menu_event.hpp"

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

namespace ct{
    enum ChoiceType{
        INPUT_FIELD,
        TOGGLE_CONFIRM,
        TOGGLE_RANGE,
        DIVIDER,
        DIVIDER_LABEL
    };
}

struct Choice{
    std::string label;
    std::string data;

    int value, min, max;

    ct::ChoiceType choiceType;
public:
    Choice(std::string label, std::string data, int value, int min, int max,
           ct::ChoiceType choiceType){
        this->label = label; this->data = data; this->value = value;
        this->min = min; this->max = max; this->choiceType = choiceType;
    };
};

class Menu{
protected:
    std::string menuName;
    std::vector<Choice> choiceList;
    std::vector<int> selections;

    int nrows;
    int ncols;
    
    int currentChoice;
private:
    virtual void toggleMenuSelection(int value){};
    virtual void confirmMenuSelection(){};
    virtual void selectNext(int value){};
    virtual void selectPrev(int value){};
    virtual void selectNextCol(){};
    virtual void selectPrevCol(){};
public:
    virtual std::string inputLabel(int xt, int yt);
public:
    Menu(){};
    virtual ~Menu(){};
  
    virtual void event(mn::EventType eType, mn::EventData eData){};
    virtual void update(){};
    virtual void render();
};

class MainMenu : public Menu{
private:
    // void toggleMenuSelection(int value);
    // void confirmMenuSelection();
    // void selectNext();
    // void selectPrev();
    // void selectNextCol();
    // void selectPrevCol();
public:
    MainMenu(){}
    virtual ~MainMenu(){};
    void event(mn::EventType eType){};
    void render();
};

class StatsMenu : public Menu{
    void toggleMenuSelection(int value);
    void confirmMenuSelection();
    void selectNext(int value);
    void selectPrev(int value);
    void selectNextCol(){};
    void selectPrevCol(){};
private:
    std::string fetchLabel(Choice choice);
public:
    StatsMenu();
    virtual ~StatsMenu();
    void event(mn::EventType eType, mn::EventData eData);
    void render();
};

#endif
