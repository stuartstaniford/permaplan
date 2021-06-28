// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is an abstract superclass for menus and panels in the menu interface
// system.  Individual menus should inherit from this and conform to this interface

#ifndef MENU_PANEL_H
#define MENU_PANEL_H

// =======================================================================================
// Class variable initialization

class MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuPanel(void);
  ~MenuPanel(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuPanel(const MenuPanel&);                 // Prevent copy-construction
  MenuPanel& operator=(const MenuPanel&);      // Prevent assignment

};

#endif




