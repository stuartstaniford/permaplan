// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class handles the UI panel for inserting a block.

#ifndef MENU_BLOCK_PANEL_H
#define MENU_BLOCK_PANEL_H

#include "MenuPanel.h"


// =======================================================================================
// Class variable initialization

class MenuBlockPanel: public MenuPanel
{
  friend MenuInterface;
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuBlockPanel(MenuInterface* menu, Scene* S);
  virtual ~MenuBlockPanel(void);
  void    display(void);

private:
  
  // Instance variables - private
  char    sizeBuf[16];
  float   size;

  // Member functions - private
  MenuBlockPanel(const MenuBlockPanel&);                 // Prevent copy-construction
  MenuBlockPanel& operator=(const MenuBlockPanel&);      // Prevent assignment
};

#endif




