// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -

#ifndef TREE_WINDOW_H
#define TREE_WINDOW_H

#include "ObjectInspectorWindow.h"


// =======================================================================================
// Useful constants etc

#define TREE_WINDOW_INIT_WIDTH  400
#define TREE_WINDOW_INIT_HEIGHT 600


// =======================================================================================
/// @brief Inspector window for a Tree.
///
/// A window to look in detail at a particular tree, abstracted from the landscape in 
/// which it is embedded.

class TreeWindow: public ObjectInspectorWindow
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  TreeWindow(void);
  ~TreeWindow(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  TreeWindow(const TreeWindow&);       
  /// @brief Prevent assignment.
  TreeWindow& operator=(const TreeWindow&);      

};

#endif




