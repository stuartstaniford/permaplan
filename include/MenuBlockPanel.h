// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class handles the UI panel for inserting a shed.

#ifndef MENU_BLOCK_PANEL_H
#define MENU_BLOCK_PANEL_H

#include "MenuPanel.h"

// =======================================================================================
// Class variable initialization

class MenuBlockPanel: public MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuBlockPanel(void);
  ~MenuBlockPanel(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuBlockPanel(const MenuBlockPanel&);                 // Prevent copy-construction
  MenuBlockPanel& operator=(const MenuBlockPanel&);      // Prevent assignment

};

#endif




