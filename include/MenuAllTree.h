// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_ALL_TREE_H
#define MENU_ALL_TREE_H

#include "MenuPanel.h"


// =======================================================================================
// Forward declarations

class RegionList;
class DynamicallyTypable;


// =======================================================================================
/// @brief Select from any tree known to permaplan
///
/// This class implements a set of menu's (based on JSON files) that ultimately aspire 
/// to be able to select any tree species in the world, and currently can select for any
/// for which we have an OTDL description.

class MenuAllTree: public MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuAllTree(MenuInterface* menu, Scene* S);
  virtual ~MenuAllTree(void);
  
  // Our abstract interface operations
  void allTreeSelection(const char* name, DynamicallyTypable* value);

  // API coming from MenuPanel and overridden here.
  void        imGuiDisplay(void);
  bool        handleHTTPRequest(HttpDebug* serv, char* path);
  ActionType  processAction(InterfaceAction* action);
  bool        handleOptionRequest(HttpDebug* serv, char* path);

private:
  
  // Instance variables - private
  RegionList* currentList;        // Effectively protected by the scene lock.

  // Member functions - private
  MenuAllTree(const MenuAllTree&);                 // Prevent copy-construction
  MenuAllTree& operator=(const MenuAllTree&);      // Prevent assignment

};


// =======================================================================================

#endif




