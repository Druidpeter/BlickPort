#include "blickport.hpp"
#include "menuparser.hpp"
#include "menu.hpp"
#include "entity.hpp"

MenuParser::MenuParser()
{

    
}

MenuParser::~MenuParser()
{

    
}

void MenuParser::parseMenu(MainMenu *menu)
{
    
}

void MenuParser::parseMenu(StatsMenu *menu)
{
    // We run through the state of the provided StatsMenu, and set
    // player variables accordingly.

    // Need to actually implement character (both pc and npc/monster)
    // stats system as a datacage in order to do this right.

    // The player entity only keeps track of it's assigned StatsState
    // through the states vector interface.

    // The stats data cage is responsible for instantiating StatsState
    // structures. It keeps track of them and also provides the
    // foundation for more powerful abstractions to be implemented
    // down the road.

    // These more powerful abstractions will likely be implemented by
    // way of Decorator friend classes. :3
}
