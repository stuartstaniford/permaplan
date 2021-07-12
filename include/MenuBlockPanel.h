// Copyright Staniford Systems.  All Rights Reserved.  June 2021 -

#ifndef MENU_BLOCK_PANEL_H
#define MENU_BLOCK_PANEL_H

#include "MenuPanel.h"


// =======================================================================================
/// @brief This class handles the UI panel for inserting a block.
///
/// This panel is invoked from the main Insert menu, and is responsible for determining
/// all the things about the inserted block required for us to create and instantiate a
/// suitable instance of the Box class (size, material, etc).

class MenuBlockPanel: public MenuPanel
{
  friend MenuInterface;
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuBlockPanel(MenuInterface* menu, Scene* S);
  virtual ~MenuBlockPanel(void);
  void    imGuiDisplay(void);

private:
  
  // Instance variables - private
  char    sizeBuf[16];
  float   size;

  // Member functions - private
  MenuBlockPanel(const MenuBlockPanel&);                 // Prevent copy-construction
  MenuBlockPanel& operator=(const MenuBlockPanel&);      // Prevent assignment
};

#endif




