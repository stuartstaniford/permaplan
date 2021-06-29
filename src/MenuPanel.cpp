// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is an abstract superclass for menus and panels in the menu interface
// system.  Individual menus should inherit from this and conform to this interface

#include "MenuPanel.h"
#include <err.h>

// =======================================================================================
// Constructor

MenuPanel::MenuPanel(MenuInterface* menu):
                  displayVisible(false),
                  mainMenu(menu)
{
}


// =======================================================================================
// Destructor

MenuPanel::~MenuPanel(void)
{
}


// =======================================================================================
// Display function - should always be overriden by subclass.

void MenuPanel::display(void)
{
  err(-1, "Unimplemented superclass MenuPanel::diaplsy");
}


// =======================================================================================
