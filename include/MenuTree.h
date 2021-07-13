// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_TREE_H
#define MENU_TREE_H


// =======================================================================================
/// @brief Floating menu to select a particular tree to insert (by latin/scientific name).
///
/// This is the menu that comes up when "Tree" is initially selected from the main 
/// insert menu, and allows the user to select from amongst the genera that have already 
/// been used in this scene.

class MenuTree
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuTree(void);
  ~MenuTree(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuTree(const MenuTree&);                 // Prevent copy-construction
  MenuTree& operator=(const MenuTree&);      // Prevent assignment

};

#endif




