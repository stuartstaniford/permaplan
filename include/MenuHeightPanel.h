// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_HEIGHT_H
#define MENU_HEIGHT_H

#include "MenuPanel.h"


// =======================================================================================
/// @brief Accept a new height estimate at a particular location.
///
/// This panel appears on clicking "Height" from the insert menu, but should also be
/// available from the HTTP interface, and in particular it's anticipated in future that
/// the most used entry point to this interface will be via a phone app using GPS.

class MenuHeightPanel: public MenuPanel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuHeightPanel(MenuInterface* menu, Scene* S);
  virtual ~MenuHeightPanel(void);
  
  // Our abstract interface operations
  void  heightEntered(float z);

  // API coming from MenuPanel and overridden here.
  void        imGuiDisplay(void);
  ActionType  processAction(InterfaceAction* action);
  bool        handleHTTPRequest(HttpDebug* serv, char* path);

private:
  
  // Instance variables - private
  char          heightBuf[16];

  // Member functions - private
  MenuHeightPanel(const MenuHeightPanel&);                 // Prevent copy-construction
  MenuHeightPanel& operator=(const MenuHeightPanel&);      // Prevent assignment

};


// =======================================================================================

#endif




