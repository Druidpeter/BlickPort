#include <iostream>

#include "blickport.hpp"
#include "menu.hpp"
#include "event.hpp"

extern BlickPort blickPort;

void MenuFactory::constructMainMenu(Menu *ob, int isUnlocked = false)
{
    ob->addChoice(std::string("New Game"), std::string("NEW_GAME"));
    ob->addChoice(std::string("Continue"), std::string("CONTINUE"));
    ob->addChoice(std::string("Options"), std::string("OPTIONS"));

    if(isUnlocked){
        ob->addChoice(std::string("Letter Box"), std::string("LETTER_BOX"));
        ob->addChoice(std::string("Level Editor"), std::string("LVL_EDITOR"), false);
    }
}

Menu *MenuFactory::getMenu(std::string choice)
{
    Menu *ob = new Menu;
    
    if(choice == "MAIN_MENU"){
        constructMainMenu(ob);
    } else if(choice == "MAIN_MENU_UNLOCKED"){
        constructMainMenu(ob, true);
    } else if(choice == "OPTIONS_MENU"){
        constructOptionsMenu(ob);
    } else if(choice == "PAUSE_MENU"){
        constructPauseMenu(ob);
    } else {
        delete ob;
        ob = NULL;

        std::cerr << "Unable to construct unknown menu! Abort.";
        exit(-10);
    }

    ob->setCurrentChoice(0);

    return ob;
}

void Menu::addChoice(std::string data, std::string label, int isSelectable,
                     int value, int min, int max)
{
    int index = choiceList.size();
    choiceList.resize(index + 1);

    Choice tmp = choiceList[index];

    tmp.data = data;
    tmp.label = label;

    tmp.value = value;
    tmp.min = min;
    tmp.max = max;
}

void Menu::event(gs::Event *event)
{
    
}

void Menu::update()
{
    
}

void Menu::render()
{
    
}
