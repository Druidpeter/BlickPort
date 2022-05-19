#include <iostream>

#include "blickport.hpp"
#include "menu.hpp"
#include "entity.hpp"
#include "curses.h"


extern BlickPort blickPort;
extern Player player;

/* Private Menu Methods */
void Menu::render()
{
    
}

/* Public Menu Methods */

void MainMenu::render()
{

}

void StatsMenu::confirmMenuSelection()
{
    int tmp = choiceList.size();
    int stats[tmp];
    
    for(int i = 0; i < tmp; i++){
        stats[i] = choiceList[i].value;
    }
    
    player.setBaseStats(stats);

    bp::EventData eData;
    blickPort.raiseEvent(bp::EventType::GOTO_MODE_GAME_LEVEL, eData);
}

StatsMenu::StatsMenu()
{    
    menuName = "Choose your Attributes: ";
    
    Choice c1("Name: ", "", 0, 0, 0, ct::INPUT_FIELD);
    Choice c2("Fitness: ", "", 0, 0, 0, ct::TOGGLE_RANGE);
    Choice c3("Dexterity: ", "", 0, 0, 0, ct::TOGGLE_RANGE);
    Choice c4("Toughness: ", "", 0, 0, 0, ct::TOGGLE_RANGE);
    Choice c5("Wit: ", "", 0, 0, 0, ct::TOGGLE_RANGE);
    Choice c6("Charisma: ", "", 0, 0, 0, ct::TOGGLE_RANGE);
    Choice c7("Willpower: ", "", 0, 0, 0, ct::TOGGLE_RANGE);
    Choice c8("Affinity: ", "", 0, 0, 0, ct::TOGGLE_RANGE);
    
    choiceList.push_back(c1);
    choiceList.push_back(c2);
    choiceList.push_back(c3);
    choiceList.push_back(c4);
    choiceList.push_back(c5);
    choiceList.push_back(c6);
    choiceList.push_back(c7);
    choiceList.push_back(c8);

    nrows = 8;
    ncols = 1;

    currentChoice = 0;
}

void StatsMenu::event(mn::EventType eType)
{
    switch(eType){
    case mn::CONFIRM_MENU_SELECTION:
        confirmMenuSelection();
        break;
    case mn::TOGGLE_SELECTION:
        toggleMenuSelection();
        break;
    case mn::SELECT_NEXT:
        selectNext();
        break;
    case mn::SELECT_PREV:
        selectPrev();
        break;
    case mn::SELECT_NEXT_COL:
        selectNextCol();
        break;
    case mn::SELECT_PREV_COL:
        selectPrevCol();
        break;
    default:
        break;
    }
}

StatsMenu::~StatsMenu()
{
    
}

void StatsMenu::render()
{
    int x = 0;
    int y = 0;
    
    const char *title = "Select Your Stats: ";
    int centering = (sizeof(title)/sizeof(char))/2;

    int xt = COLS/2 - centering;
    int yt = LINES/2 - choiceList.size()/2 - 2;

    mvaddstr(yt, xt, title);
    yt += 2;
}
