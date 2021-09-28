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


class MenuGablePanel: public MenuPanel, public GableParamData
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuGablePanel(MenuInterface* menu, Scene* S);
  virtual ~MenuGablePanel(void);
  
  // Our abstract interface operations
  void gableEntered(void);

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
  static char  overhangLabel[16];

  // Member functions - private
  MenuGablePanel(const MenuGablePanel&);                 // Prevent copy-construction
  MenuGablePanel& operator=(const MenuGablePanel&);      // Prevent assignment

};


// =======================================================================================

#endif




