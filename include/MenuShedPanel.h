// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef MENU_SHED_PANEL_H
#define MENU_SHED_PANEL_H

#include "MenuPanel.h"
#include "Shed.h"


// =======================================================================================
/// @brief The floating menu to set up parameters for an inserted shed
/// 
/// This class provides for the display of a single UI panel that can be used when
/// inserting or editing a shed building (or shed-like component of a larger building.) 

class MenuShedPanel: public MenuPanel, public ShedParamData
{  
  friend MenuInterface;
  friend Shed;

public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuShedPanel(MenuInterface* menu, Scene* S);
  virtual ~MenuShedPanel(void);

  // Our abstract interface operations
  void shedEntered(void);

  // API coming from MenuPanel and overridden here.
  void    imGuiDisplay(void);
  char*   errorInFields(void);
  ActionType processAction(InterfaceAction* action);
  bool handleHTTPRequest(HttpDebug* serv, char* path);

private:

  // static buffers for labels for controls
  static bool  staticsInitialized;
  static char  heightLabel[16];
  static char  lengthLabel[16];
  static char  widthLabel[16];
  static char  rOverhangLabel[16];
  static char  lOverhangLabel[16];
  static char  fOverhangLabel[16];
  static char  bOverhangLabel[16];

  // Member functions - private
  MenuShedPanel(const MenuShedPanel&);                 // Prevent copy-construction
  MenuShedPanel& operator=(const MenuShedPanel&);      // Prevent assignment

};

#endif




