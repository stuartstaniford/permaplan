// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class if for an error warning that needs to be thrown up and block the interface
// (eg because there are bad parameters in another panel, and the user needs to confirm
// their intent to fix them before continuing).

#include "MenuErrorPanel.h"


// =======================================================================================
// Constructor

MenuErrorPanel::MenuErrorPanel(MenuInterface* menu):
                                  MenuPanel(menu)
{
}


// =======================================================================================
// Destructor

MenuErrorPanel::~MenuErrorPanel(void)
{
}


// =======================================================================================
