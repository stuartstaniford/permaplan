// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_FOCUS_OVERLAY_H
#define MENU_FOCUS_OVERLAY_H

#include "MenuPanel.h"


// =======================================================================================
// Forward declarations

class Window3D;


// =======================================================================================
/// @brief Transparent overlay that displays information about the thing currently in 
/// focus.
///
/// Currently this is just informational and has no actions, so it doesn't implement
/// things like handleHTTPRequest and processAction.

class MenuFocusOverlay: public MenuPanel
{
public:
  
  // Instance variables - public
  Window3D& win3D;
  
  // Member functions - public
  MenuFocusOverlay(MenuInterface* menu, Scene* S, Window3D& W);
  ~MenuFocusOverlay(void);
  void  imGuiDisplay(void);

private:
  
  // Instance variables - private
  
  // Member functions - private
  void mouseOverlayDisplays(vec3 mouseSceneLoc);
  MenuFocusOverlay(const MenuFocusOverlay&);                 // Prevent copy-construction
  MenuFocusOverlay& operator=(const MenuFocusOverlay&);      // Prevent assignment
};


// =======================================================================================

#endif




