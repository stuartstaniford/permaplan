// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_FOCUS_OVERLAY_H
#define MENU_FOCUS_OVERLAY_H

// =======================================================================================
/// @brief Transparent overlay that displays information about the thing currently in focus

class MenuFocusOverlay
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuFocusOverlay(void);
  ~MenuFocusOverlay(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuFocusOverlay(const MenuFocusOverlay&);                 // Prevent copy-construction
  MenuFocusOverlay& operator=(const MenuFocusOverlay&);      // Prevent assignment
};

#endif




