#include <iostream>

#include "blickport.hpp"
#include "menu.hpp"
#include "menuparser.hpp"
#include "event.hpp"
#include "curses.h"

extern BlickPort blickPort;
extern MenuParser menuParser;

void MenuFactory::constructMainMenu(Menu *ob)
{
    ob->addChoice(std::string("New Game"), false, true,
                  0, 0, 1);
    ob->addChoice(std::string("Continue"), false, true,
                  0, 0, 1);
    ob->addChoice(std::string("Options"), false, true,
                  0, 0, 1);

    ob->setCurrentChoice(0);
}

void MenuFactory::constructStatsMenu(Menu *ob)
{
    ob->addChoice(std::string("Name: "), true);
    
    ob->addDivider();
    
    ob->addChoice(std::string("Fitness: "), false, false, 0, 0, 10);
    ob->addChoice(std::string("Dexterity: "), false, false, 0, 0, 10);
    ob->addChoice(std::string("Toughness: "), false, false, 0, 0, 10);
    ob->addChoice(std::string("Wit: "), false, false, 0, 0, 10);
    ob->addChoice(std::string("Charisma: "), false, false, 0, 0, 10);
    ob->addChoice(std::string("Willpower: "), false, false, 0, 0, 10);

    ob->setCurrentChoice(0);
}

Menu *MenuFactory::getMenu(std::string choice)
{
    Menu *ob;
    
    if(choice == "MAIN_MENU"){
        ob = new MainMenu();
        constructMainMenu(ob);
        ob->setName(choice.c_str());
    } else if(choice == "MAIN_MENU_UNLOCKED"){
        //constructMainMenu(ob);
        return NULL;
    } else if(choice == "OPTIONS_MENU"){
        //constructOptionsMenu(ob);
        return NULL;
    } else if(choice == "PAUSE_MENU"){
        //constructPauseMenu(ob);
        return NULL;
    } else if(choice == "STATS_MENU"){
        ob = new StatsMenu();
        constructStatsMenu(ob);
        ob->setName(choice.c_str());
    } else {
        std::cerr << "Unable to construct unknown menu! Abort.";
        exit(-10);
    }

    // ob->setName(choice.c_str());
    
    return ob;
}

/* Private Menu Methods */

void Menu::toggleCurrentChoice(std::string value)
{
    Choice *tmp = &choiceList[currentChoice];

    if(value.compare("left") == 0 && tmp->inputField == false){
        tmp->value = (tmp->value - 1 < tmp->min) ? (tmp->min) : (--(tmp->value));

    } else if(value.compare("right") == 0 && tmp->inputField == false){
        tmp->value = (tmp->value + 1 < tmp->max) ? (++(tmp->value)) : (tmp->max);
        
    } else if(value.compare("left") == 0 && tmp->inputField == true){
        tmp->data.clear();
        
    }  else if(value.compare("right") == 0 && tmp->inputField == true){

        // There's no way to move the cursor to the position of the
        // field for input, as this method doesn't have access to any
        // of the rendering information.

        // So instead, we will need to create a field input bar at the
        // bottom of the terminal, kind of like the way they do in
        // emacs, for inputting data to fields of the current choice.

        // This field input bar should also contain hints for how to
        // navigate the menu. e.g. "Press right arrow or 'l' to
        // input/toggle options." So it's a good idea to implement in
        // general.

        // But... for now, it's ok if we just leave ugly echoing on
        // and implement the bar later. Implementing the bar currently
        // falls under the "polish and finishing up" category. There
        // are other things on the TODO list that are far more
        // important. But we'll get to it. :D
        
        echo();
        char input[32];
        getnstr(input, 31);
        noecho();

        tmp->data = input;
    }

    if(value.compare("LEFT") == 0 && tmp->isSelectable == true){
        int sindex;
        for(int i = 0; i < selections.size(); ++i){
            if(selections[i] == currentChoice){
                std::swap(selections[i], selections.back());
                selections.pop_back();
                break;
            }
        }
    } else if(value.compare("RIGHT") == 0 && tmp->isSelectable == true){
        if(requireUnique == true){
            selections.clear();
        } selections.push_back(currentChoice);
    }
}

void Menu::moveSelection(int value)
{
    int wrap = choiceList.size();
    currentChoice = (currentChoice + value) % wrap;

    if(requireUnique == true){
        if(selections.size() > 0){
            selections.clear();
        } selections.push_back(currentChoice);
    }
}


/* Public Menu Methods */

void Menu::addChoice(std::string label, int inputField, int isSelectable,
                     int value, int min, int max)
{
    int index = choiceList.size();
    choiceList.resize(index + 1);

    Choice *tmp = &choiceList[index];

    tmp->label = label;
    tmp->inputField = inputField;
    tmp->isSelectable = isSelectable;
    tmp->value = value;
    tmp->min = min;
    tmp->max = max;
}

void Menu::addDivider()
{
    dividers.push_back(choiceList.size() - 1);
}

void Menu::event(gs::Event *event)
{

}

void Menu::event(int event)
{
    switch(event){
    case CONFIRM_MENU_SELECTION:

        // NOTE: Right now, we are handling menu confirmation logic
        // inside this method. However, we need to push the logic out
        // to polymorphic virtual functions for each type of menu.

        // We need to do this because the menuParser object overloads
        // parseMenu() for each type of derived menu class. The
        // derived menu class will know what subtype it is, and needs
        // to cast itself to its subtype when passing itself as a
        // parameter to the parseMenu() method. This way, the
        // menuParser can correctly handle all types of menus, and we
        // don't have to have two parallel systems of base and derived
        // menu classes and menu handler classes.

        // It's good practice to keep inheritance trees as shallow as
        // possible.
        
        if(menuName.compare("MAIN_MENU") == 0){
            blickPort.raiseEvent(GOTO_MODE_STATS_MENU);
        } else if(menuName.compare("STATS_MENU") == 0){
            blickPort.raiseEvent(GOTO_MODE_GAME_LEVEL);
        } else {
            return;
        } break;
    case TOGGLE_SELECTION_LEFT:
        toggleCurrentChoice("left");
        break;
    case TOGGLE_SELECTION_RIGHT:
        toggleCurrentChoice("right");
        break;
    case MOVE_SELECTION_PREV:
        moveSelection(-1);
        break;
    case MOVE_SELECTION_NEXT:
        moveSelection(1);
        break;
    default:
        break;
    }
}

void Menu::update()
{
    
}

void Menu::render(int x, int y)
{
    Choice *choice;
    
    for(int i = 0; i < choiceList.size(); ++i){
        choice = &choiceList[i];
        mvaddstr(y, x, choice->label.c_str());

        int tmpx = 9;

        if(i == currentChoice){
            mvaddch(y, x-2, '*');
        }
        
        if(choice->inputField == true){
            mvaddstr(y, x + tmpx + 2, choice->data.c_str());
        } else {
            const char *minstr = std::to_string(choice->min).c_str();
            const char *maxstr = std::to_string(choice->max).c_str();
            
            int sminstr = sizeof(minstr)/sizeof(char);
            int smaxstr = sizeof(maxstr)/sizeof(char);

            int tmp2 = x+tmpx+sminstr;
            
            mvaddstr(y, x+tmpx+2, minstr);
            mvaddch(y, tmp2, '|');
            
            for(int j = choice->min; j <= choice->max; ++j){
                tmp2++;
                
                if(j <= choice->value){
                    mvaddch(y, tmp2, '-');
                } else {
                    mvaddch(y, tmp2, '.');
                }
            } tmp2++;

            mvaddch(y, tmp2, '|');
            tmp2 += 2;
            mvaddstr(y, tmp2, maxstr);
        }

        y += 1;
    }
}

void MainMenu::render(int x, int y)
{
    const char *title = "BlickPort::v0.1a";
    int centering = (sizeof(title)/sizeof(char))/2;

    int xt = COLS/2 - centering;
    int yt = LINES/2 - choiceList.size()/2 - 2;

    mvaddstr(yt, xt, title);
    yt += 2;

    Menu::render(xt, yt);
}

void StatsMenu::render(int x, int y)
{
    const char *title = "Select Your Stats: ";
    int centering = (sizeof(title)/sizeof(char))/2;

    int xt = COLS/2 - centering;
    int yt = LINES/2 - choiceList.size()/2 - 2;

    mvaddstr(yt, xt, title);
    yt += 2;

    Menu::render(xt, yt);
}
