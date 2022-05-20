#include <iostream>

#include "blickport.hpp"
#include "menu.hpp"
#include "entity.hpp"
#include "curses.h"


extern BlickPort blickPort;
extern Player player;

std::string Menu::fetchLabel(Choice choice)
{
    std::string tmp = std::to_string(choice.value);
    tmp.append(" |");

    for(int i=0; i < choice.value; ++i){
        tmp.append("*");
    }

    for(int i=0; i < choice.max - choice.value; ++i){
        tmp.append("-");
    } tmp.append("|");

    return tmp;
}

std::string Menu::inputLabel(int xt, int yt)
{
    // Move ncurses cursor to xt, yt.
    
    // Enable Ncurses Character Echo Input.
    
    // Then get line input from user. Ends with 'Enter'.

    // Disable character echo.

    // Return line input as a string.

    // For now, just return an empty string.
    std::string rvalue = "";
    return rvalue;
}

void Menu::selectNext(int value)
{
    int tmp = currentChoice;
    int length = choiceList.size();
    
    while(value > 0){
        tmp++;

        if(tmp >= length){
            tmp = 0;
        }

        while(choiceList[tmp].choiceType == ct::DIVIDER ||
              choiceList[tmp].choiceType == ct::DIVIDER_LABEL){
            tmp += (tmp < length) ? 1 : -tmp;
        }

        --value;
    } currentChoice = tmp;
}

void Menu::selectPrev(int value)
{
    int tmp = currentChoice;
    int length = choiceList.size();
    
    while(value > 0){
        tmp--;

        if(tmp < 0){
            tmp = length - 1;
        }

        while(choiceList[tmp].choiceType == ct::DIVIDER ||
              choiceList[tmp].choiceType == ct::DIVIDER_LABEL){
            tmp += (tmp > 0) ? -1 : (length - 1);
        }

        --value;
    } currentChoice = tmp;
}

void Menu::event(mn::EventType eType, mn::EventData eData)
{
    switch(eType){
    case mn::TOGGLE_SELECTION:
    case mn::CONFIRM_MENU_SELECTION:
        confirmMenuSelection();
        break;
    case mn::CLEAR_SELECTION:
        // Go to previous menu.
        break;
    case mn::SELECT_NEXT:
        selectNext(1);
        break;
    case mn::SELECT_PREV:
        selectPrev(1);
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


/* Private Menu Methods */
void Menu::render()
{
    int x = 0;
    int y = 0;
    
    int centering = (menuName.size())/2;

    int xt = COLS/2 - centering;
    int yt = LINES/2 - choiceList.size()/2 - 2;

    // Yeah, I know, it's bad form to hard code lt. Whatever. Sue
    // me.
    const int lt = 12;

    mvaddstr(yt, xt, menuName.c_str());
    yt++;

    std::string lbl;
    
    for(int i = 0; i < choiceList.size(); i++){
        yt++;

        if(i == currentChoice){
            mvaddstr(yt, xt-2, "*");
        }
        
        mvaddstr(yt, xt, choiceList[i].label.c_str());

        if(choiceList[i].choiceType == ct::TOGGLE_RANGE){
            lbl = fetchLabel(choiceList[i]);
        } else if(choiceList[i].choiceType == ct::INPUT_FIELD){
            int xtt = xt + choiceList[i].label.size() + 2;
            lbl = inputLabel(xtt, yt);
        }
        
        mvaddstr(yt, xt + lt, lbl.c_str());
    }
}
/* Public Menu Methods */

MainMenu::MainMenu()
{
    menuName = "BlickPort v0.1a";

    Choice c1("New Game", "", 0, 0, 0, ct::TOGGLE_CONFIRM);
    Choice c2("Continue", "", 0, 0, 0, ct::TOGGLE_CONFIRM);
    Choice c3("Options", "", 0, 0, 0, ct::TOGGLE_CONFIRM);

    choiceList.push_back(c1);
    choiceList.push_back(c2);
    choiceList.push_back(c3);

    nrows = 3;
    ncols = 1;
}

void MainMenu::confirmMenuSelection()
{
    // Since we don't have Continue nor Options functionality yet,
    // just always start a new game.

    bp::EventData eData;
    blickPort.raiseEvent(bp::EventType::INIT_NEW_GAME, eData);

    blickPort.raiseEvent(bp::EventType::GOTO_STATS_MENU, eData);
}

void MainMenu::event(mn::EventType eType, mn::EventData eData)
{
    switch(eType){
    case mn::CONFIRM_MENU_SELECTION:
        confirmMenuSelection();
        break;
    case mn::TOGGLE_SELECTION:
        break;
    case mn::CLEAR_SELECTION:
        break;
    case mn::SELECT_NEXT:
        selectNext(1);
        break;
    case mn::SELECT_PREV:
        selectPrev(1);
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

void StatsMenu::toggleMenuSelection(int value)
{
    if(value > 0){
        choiceList[currentChoice].value +=
            (choiceList[currentChoice].value <=
             choiceList[currentChoice].max - value) ? value : 0;
    } else if(value < 0){
        choiceList[currentChoice].value +=
            (choiceList[currentChoice].value >=
             choiceList[currentChoice].min - value) ? value : 0;        
    }
}

void StatsMenu::selectNext(int value)
{
    int tmp = currentChoice;
    int length = choiceList.size();
    
    while(value > 0){
        tmp++;

        if(tmp >= length){
            tmp = 0;
        }

        while(choiceList[tmp].choiceType == ct::DIVIDER ||
              choiceList[tmp].choiceType == ct::DIVIDER_LABEL){
            tmp += (tmp < length) ? 1 : -tmp;
        }

        --value;
    } currentChoice = tmp;
}

void StatsMenu::selectPrev(int value)
{
    int tmp = currentChoice;
    int length = choiceList.size();
    
    while(value > 0){
        tmp--;

        if(tmp < 0){
            tmp = length - 1;
        }

        while(choiceList[tmp].choiceType == ct::DIVIDER ||
              choiceList[tmp].choiceType == ct::DIVIDER_LABEL){
            tmp += (tmp > 0) ? -1 : (length - 1);
        }

        --value;
    } currentChoice = tmp;
    
}

std::string StatsMenu::fetchLabel(Choice choice)
{
    std::string tmp = std::to_string(choice.value);
    tmp.append(" |");

    for(int i=0; i < choice.value; ++i){
        tmp.append("*");
    }

    for(int i=0; i < choice.max - choice.value; ++i){
        tmp.append("-");
    } tmp.append("|");

    return tmp;
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
    blickPort.raiseEvent(bp::EventType::GOTO_RACE_MENU, eData);
}

StatsMenu::StatsMenu()
{    
    menuName = "Choose your Attributes: ";
    
    Choice c1("Name: ", "", 0, 0, 0, ct::INPUT_FIELD);
    Choice c2("Fitness: ", "", 6, 0, 10, ct::TOGGLE_RANGE);
    Choice c3("Dexterity: ", "", 5, 0, 10, ct::TOGGLE_RANGE);
    Choice c4("Toughness: ", "", 6, 0, 10, ct::TOGGLE_RANGE);
    Choice c5("Wit: ", "", 5, 0, 10, ct::TOGGLE_RANGE);
    Choice c6("Charisma: ", "", 6, 0, 10, ct::TOGGLE_RANGE);
    Choice c7("Willpower: ", "", 5, 0, 10, ct::TOGGLE_RANGE);
    Choice c8("Affinity: ", "", 6, 0, 10, ct::TOGGLE_RANGE);
    
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

void StatsMenu::event(mn::EventType eType, mn::EventData eData)
{
    switch(eType){
    case mn::CONFIRM_MENU_SELECTION:
        confirmMenuSelection();
        break;
    case mn::TOGGLE_SELECTION:
        toggleMenuSelection(1);
        break;
    case mn::CLEAR_SELECTION:
        toggleMenuSelection(-1);
        break;
    case mn::SELECT_NEXT:
        selectNext(1);
        break;
    case mn::SELECT_PREV:
        selectPrev(1);
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

    // Yeah, I know, it's bad form to hard code lt. Whatever. Sue
    // me.
    const int lt = 12;

    mvaddstr(yt, xt, title);
    yt++;

    std::string lbl;
    
    for(int i = 0; i < choiceList.size(); i++){
        yt++;

        if(i == currentChoice){
            mvaddstr(yt, xt-2, "*");
        }
        
        mvaddstr(yt, xt, choiceList[i].label.c_str());

        if(choiceList[i].choiceType == ct::TOGGLE_RANGE){
            lbl = fetchLabel(choiceList[i]);
        } else if(choiceList[i].choiceType == ct::INPUT_FIELD){
            int xtt = xt + choiceList[i].label.size() + 2;
            lbl = inputLabel(xtt, yt);
        }
        
        mvaddstr(yt, xt + lt, lbl.c_str());
    }
}

void RaceMenu::confirmMenuSelection()
{
    Choice race = choiceList[currentChoice];

    // Note: For now, we just use race.value as integers. However, we
    // should eventually create a 'defines.hpp' that defines various
    // global enumeration values that are just useful in situations
    // like this, and that are understood across the program.

    // This is no place for wasting dev time on creating byzantine
    // enum class systems. ;^D
    
    player.setRace(race.value);

    bp::EventData eData;
    blickPort.raiseEvent(bp::EventType::GOTO_CLASS_MENU, eData);
}


RaceMenu::RaceMenu()
{
    menuName = "Choose your Race: ";

    Choice c1("Human", "", RACE_HUMAN, 0, 0, ct::TOGGLE_CONFIRM);
    choiceList.push_back(c1);

    nrows = 1;
    ncols = 1;
}

void ClassMenu::confirmMenuSelection()
{
    Choice classChoice = choiceList[currentChoice];
    player.setClass(classChoice.value);

    bp::EventData eData;
    blickPort.raiseEvent(bp::EventType::GOTO_MODE_GAME_LEVEL, eData);
}

ClassMenu::ClassMenu()
{
    menuName = "Choose your Class: ";

    Choice c1("Noble", "", CLASS_NOBLE, 0, 0, ct::TOGGLE_CONFIRM);
    Choice c2("Tradesman", "", CLASS_TRADESMAN, 0, 0, ct::TOGGLE_CONFIRM);
    Choice c3("Soldier", "", CLASS_SOLDIER, 0, 0, ct::TOGGLE_CONFIRM);
    Choice c4("Doctor", "", CLASS_DOCTOR, 0, 0, ct::TOGGLE_CONFIRM);
    Choice c5("Scholar", "", CLASS_SCHOLAR, 0, 0, ct::TOGGLE_CONFIRM);
    Choice c6("Peasant", "", CLASS_PEASANT, 0, 0, ct::TOGGLE_CONFIRM);

    choiceList.push_back(c1);
    choiceList.push_back(c2);
    choiceList.push_back(c3);
    choiceList.push_back(c4);
    choiceList.push_back(c5);
    choiceList.push_back(c6);

    nrows = 3;
    ncols = 2;
}
