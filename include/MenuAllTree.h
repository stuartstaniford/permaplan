// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_ALL_TREE_H
#define MENU_ALL_TREE_H

#include "MenuPanel.h"


// =======================================================================================
/// @brief Quick description of the class
///
/// More details of the class go here.

class MenuAllTree: public MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuAllTree(MenuInterface* menu, Scene* S);
  ~MenuAllTree(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuAllTree(const MenuAllTree&);                 // Prevent copy-construction
  MenuAllTree& operator=(const MenuAllTree&);      // Prevent assignment

};

#endif




