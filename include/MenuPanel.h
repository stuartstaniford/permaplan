// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is an abstract superclass for menus and panels in the menu interface
// system.  Individual menus should inherit from this and conform to this interface

#ifndef MENU_PANEL_H
#define MENU_PANEL_H

#include "Logging.h"
#include "VisualObject.h"
#include "imgui.h"

// =======================================================================================
// Class variable initialization

class MenuInterface;

class MenuPanel
{
  friend MenuInterface;

public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuPanel(MenuInterface* menu);
  virtual ~MenuPanel(void);
  virtual void  display(void);
  virtual char* errorInFields(void);

protected:

  // Instance variables visible to subclasses
  
  bool            displayVisible;
  MenuInterface*  mainMenu;

private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuPanel(const MenuPanel&);                 // Prevent copy-construction
  MenuPanel& operator=(const MenuPanel&);      // Prevent assignment

};

#endif




