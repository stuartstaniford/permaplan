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
  void    imGuiDisplay(void);
  char*   errorInFields(void);
  
private:
  
  // Instance variables - private
  float height;
  float length;
  float width;
  float roofAngle;
  float overhang;  

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

#endif




