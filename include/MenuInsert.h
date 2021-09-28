// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_INSERT_H
#define MENU_INSERT_H

#include "MenuPanel.h"


// =======================================================================================
/// @brief Code for the insertion menu that comes up on double-clicking.
///
/// The job of this class is to provide a menu capable of routing us to task-specific
/// panels for all the different kinds of things that can be insderted (height 
/// measurements, building components, trees/plants, etc).

class MenuInsert: public MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuInsert(MenuInterface* menu, Scene* S);
  virtual ~MenuInsert(void);

  // Our abstract interface operations
  void  insertHeight(void);
  void  insertBlock(void);
  void  insertShed(void);
  void  insertGable(void);
  void  insertTree(void);

  // API coming from MenuPanel and overridden here.
  void        imGuiDisplay(void);
  bool        handleHTTPRequest(HttpDebug* serv, char* path);
  ActionType  processAction(InterfaceAction* action);

private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction
  MenuInsert(const MenuInsert&);                 
  /// @brief Prevent assignment
  MenuInsert& operator=(const MenuInsert&);      
};


// =======================================================================================

#endif
