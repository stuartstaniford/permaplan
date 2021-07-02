// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is an abstract superclass for menus and panels in the menu interface
// system.  Individual menus should inherit from this and conform to this interface

#include "MenuPanel.h"
#include <err.h>

// =======================================================================================
/// @brief Constructor for the MenuPanel that initializes the generic variables
/// 
/// @param menu Pointer to the overall MenuInterface so we (and our subclasses) can 
/// call back into it. 

MenuPanel::MenuPanel(MenuInterface* menu):
                  displayVisible(false),
                  mainMenu(menu),
                  errPanel(NULL)
{
}


// =======================================================================================
/// \brief Destructor
///

MenuPanel::~MenuPanel(void)
{
}


// =======================================================================================
/// @brief Interface for subclass function to check that all panel fields are complete
/// and correct.  
/// 
/// The version in this clsss will always return an error.  Subclasses should override
/// this.
///
/// @return A pointer to a char array with an error message.  It should be NULL if there
/// is no error.

char* MenuPanel::errorInFields(void)
{
  static char* errString = (char*)"Unimplemented superclass method "
                                                              "MenuPanel::errorInFields.\n";
  return errString;
}


// =======================================================================================
/// @brief Interface for displaying the panel when it is supposed to be on screen.  
/// 
/// The version in this clsss will always return an error.  Subclasses should override
/// this.

void MenuPanel::display(void)
{
  err(-1, "Unimplemented superclass MenuPanel::diaplsy");
}


// =======================================================================================
