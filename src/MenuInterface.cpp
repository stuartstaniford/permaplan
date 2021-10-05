// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// This class is the master class in charge of the whole menu interface - both the 
// ImGui windows shown on screen as well as the HTTP interface to the various options
// that is used for scripting/testing.  It is where all the pointers live that point
// to instances of all the different individuals menus and panels 

#include "MenuInterface.h"
#include <err.h>

// =======================================================================================
/// @brief Constructor for the (mostly) abstract MenuInterface class
/// @param W A reference to the Window3D with which this instance is associated.

MenuInterface::MenuInterface(Window3D& W):
                        MenuPanel(this, NULL, &W), // scene will be set later
                        win3D(W)
{
}


// =======================================================================================
/// @brief Destructor

MenuInterface::~MenuInterface(void)
{
}


// =======================================================================================
/// @brief Something has gone wrong and we need to create an errorpanel.
/// @param errString - a C string describing the problem.
/// @todo This is currently a stub with no implementation.

void  MenuInterface::createErrorPanel(const char* errString)
{
  
}


// =======================================================================================
/// @brief Top level function to render all active menus/panels.
///
/// This is the function called from the main rendering loop (Window3D::loop) to display
/// any menus or panels that should be active (which is determined by their being a
/// non-NULL pointer).  The implementation here needs to be overridden by subclasses.

void MenuInterface::imguiInterface(void)
{
  err(-1, "Call to unimplemented superclass method MenuInterface::imguiInterface");
}


// =======================================================================================
/// @brief Top level function to parse interface requests from the HTTP server.
/// 
/// This function to is to handle the parsing of menu interface actions coming from HTTP 
/// (ie for test scripts).  Everything with a URL path starting with /menu/ comes here.
/// We figure out the next component in the path and then route it to the right place.
/// Most requests will end up at the handleHTTPRequest method of some subclass of
/// MenuPanel.  The implementation here needs to be overridden by subclasses.
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuInterface::HTTPAPi(HttpDebug* serv, char* path)
{  
  err(-1, "Call to unimplemented superclass method MenuInterface::HTTPAPi");
}


// =======================================================================================
/// @brief Handle a pseudo-interface event coming from the HTTP Debug interface.  We are
/// called from the equivalent function in Window3D to handle actions that are associated
/// with the menu system.  Generally we are routing the request on to the processAction
/// method for the appropriate subclass of MenuPanel.  The implementation here needs to 
/// be overridden by subclasses.
///
/// @returns The ActionType of the action we processed.
/// @param action The InterfaceAction that needs to be handled.
/// @todo Possibly this should be handled via a std::map<ActionType, MenuPanel*>

ActionType MenuInterface::processAction(InterfaceAction* action)
{  
  err(-1, "Call to unimplemented superclass method MenuInterface::processAction");
}


// =======================================================================================
