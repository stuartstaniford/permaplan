// Copyright Staniford Systems.  All Rights Reserved.  September 2021 -

#ifndef MENU_INIT_PANEL_H
#define MENU_INIT_PANEL_H

#include "MenuPanel.h"

// =======================================================================================
/// @brief Special interface for initialization user interaction.
/// 
/// This class is a special case of MenuPanel used only during startup when we need to 
/// interact with the user about something (often something that has gone wrong).  It 
/// is only called from Window3D::initPanel, which has it's own special rendering loop 
/// because this is for use before the main rendering loop has started.

class MenuInitPanel//: public MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuInitPanel(void);
  ~MenuInitPanel(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  MenuInitPanel(const MenuInitPanel&);       
  /// @brief Prevent assignment.
  MenuInitPanel& operator=(const MenuInitPanel&);      

};

#endif




