/**
 * @file GUI.hpp
 * @brief In game menus and GUI
 *
 *
 *
 */

#ifndef __glitchspike__gui__
#define __glitchspike__gui__


//! The GUI class is a container class for non main window
//! elements. Basically, this means menus and menu-like screens. Also
//! handles character creation screens.

//! Essentially, the GUI holds references to menu objects. One or more
//! of these objects may be marked as active. When updating and
//! rendering the GUI, only active menu objects are updated and
//! rendered, as well.

//! Gameplay is always rendered and updated before GUI. This ensures
//! that GUI elements are always visible, though naturally they may
//! obscure parts of the screen.

class Gui{
  //
public:
    Gui(){};
    virtual ~Gui(){};
};

#endif
