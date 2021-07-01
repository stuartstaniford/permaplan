// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class handles the UI panel for inserting a shed.

#ifndef MENU_SHED_PANEL_H
#define MENU_SHED_PANEL_H

#include "MenuPanel.h"

// =======================================================================================
// Class variable initialization

class MenuShedPanel: public MenuPanel
{  
  friend MenuInterface;

public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuShedPanel(MenuInterface* menu);
  virtual ~MenuShedPanel(void);
  void    display(void);
  char*   errorInFields(void);

private:
  
  // Instance variables - private
  float height;
  float length;
  float width;
  float roofAngle;

  // static buffers for labels for controls
  static bool  staticsInitialized;
  static char  heightLabel[16];
  static char  lengthLabel[16];
  static char  widthLabel[16];

  // Member functions - private
  MenuShedPanel(const MenuShedPanel&);                 // Prevent copy-construction
  MenuShedPanel& operator=(const MenuShedPanel&);      // Prevent assignment

};

#endif




