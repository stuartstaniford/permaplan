// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_GABLE_PANEL_H
#define MENU_GABLE_PANEL_H

#include "MenuPanel.h"
#include "Gable.h"


// =======================================================================================
/// @brief The floating menu to set up parameters for an inserted gable
/// 
/// This class provides for the display of a single UI panel that can be used when
/// inserting or editing a gable building (or gable-like component of a larger building.) 


class MenuGablePanel: public MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuGablePanel(MenuInterface* menu, Scene* S);
  ~MenuGablePanel(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuGablePanel(const MenuGablePanel&);                 // Prevent copy-construction
  MenuGablePanel& operator=(const MenuGablePanel&);      // Prevent assignment

};

#endif




