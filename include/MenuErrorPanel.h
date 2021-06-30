// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class if for an error warning that needs to be thrown up and block the interface
// (eg because there are bad parameters in another panel, and the user needs to confirm
// their intent to fix them before continuing).

#ifndef MENU_ERROR_PANEL_H
#define MENU_ERROR_PANEL_H

#include "MenuPanel.h"

// =======================================================================================
// Class variable initialization

class MenuErrorPanel: public MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuErrorPanel(MenuInterface* menu);
  ~MenuErrorPanel(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuErrorPanel(const MenuErrorPanel&);                 // Prevent copy-construction
  MenuErrorPanel& operator=(const MenuErrorPanel&);      // Prevent assignment

};

#endif




