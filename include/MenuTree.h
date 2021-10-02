// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_TREE_H
#define MENU_TREE_H

#include "MenuPanel.h"


// =======================================================================================
// Forward declarations


// =======================================================================================
/// @brief Floating menu to select a particular tree to insert (by latin/scientific name).
///
/// This is the menu that comes up when "Tree" is initially selected from the main 
/// insert menu, and allows the user to select from amongst the genera that have already 
/// been used in this scene.

class MenuTree: public MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuTree(MenuInterface* menu, Scene* S);
  virtual ~MenuTree(void);
  
  // Our abstract interface operations
  void treeMenuSelection(const char* genusString);

  // API coming from MenuPanel and overridden here.
  void        imGuiDisplay(void);
  bool        handleHTTPRequest(HttpDebug* serv, char* path);
  ActionType  processAction(InterfaceAction* action);
  bool        handleOptionRequest(HttpDebug* serv, char* path);

private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuTree(const MenuTree&);                 // Prevent copy-construction
  MenuTree& operator=(const MenuTree&);      // Prevent assignment

};


// =======================================================================================

#endif




