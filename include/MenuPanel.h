// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef MENU_PANEL_H
#define MENU_PANEL_H

#include "Logging.h"
#include "VisualObject.h"
#include "imgui.h"
#include "Scene.h"


// =======================================================================================

class MenuInterface;
class MenuErrorPanel;

// =======================================================================================
/// @brief Provides an interface that all GUI panels/menus inherit from
/// 
/// This class is an abstract superclass for menus and panels in the menu interface
/// system.  Individual menus should inherit from this and conform to this interface. 

class MenuPanel
{
  friend MenuInterface;

public:
  
  // Instance variables - public
  Scene*          scene;
  
  // Member functions - public
  MenuPanel(MenuInterface* menu, Scene* S);
  ~MenuPanel(void);
  virtual void  display(void);
  virtual char* errorInFields(void);
  virtual bool handleHTTPRequest(HttpDebug* serv, char* path);
  void setCorner(int& corner);
  void setCenter(void);

protected:

  // Instance variables visible to subclasses
  
  bool            displayVisible;
  MenuInterface*  mainMenu;
  MenuErrorPanel* errPanel;

  // Methods visible only to subclasses  
  bool createAction(HttpDebug* serv, ActionType actionType, 
                          char* actionName, char* functionName, char* path);

private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuPanel(const MenuPanel&);                 // Prevent copy-construction
  MenuPanel& operator=(const MenuPanel&);      // Prevent assignment

};

#endif




