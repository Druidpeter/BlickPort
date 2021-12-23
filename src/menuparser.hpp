/*
 * @file menuparser.hpp
 *
 */

#ifndef __blickport__menu_parser__
#define __blickport__menu_parser__

#include "blickport.hpp"
#include "entity.hpp"

class Menu;
class MainMenu;
class StatsMenu;

extern Menu *activeMenu;
extern BlickPort blickPort;
extern Player player;

class MenuParser{
    
public:
    MenuParser();
    ~MenuParser();

    void parseMenu(MainMenu *menu);
    void parseMenu(StatsMenu *menu);

    // We'll add additional parseMenu() methods overloaded for each
    // menu derived class as they are implemented.
};


#endif
