// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is an abstract superclass for menus and panels in the menu interface
// system.  Individual menus should inherit from this and conform to this interface

#include "MenuPanel.h"
#include <err.h>

// =======================================================================================
/// @brief Constructor for the MenuPanel that initializes the generic variables
/// 
/// @param menu Pointer to the overall MenuInterface so we (and our subclasses) can 
/// call back into it. 

MenuPanel::MenuPanel(MenuInterface* menu, Scene* S):
                  scene(S),
                  displayVisible(false),
                  mainMenu(menu),
                  errPanel(NULL)
{
}


// =======================================================================================
/// @brief Destructor
///

MenuPanel::~MenuPanel(void)
{
}


// =======================================================================================
/// @brief Interface for subclass function to check that all panel fields are complete
/// and correct.  
/// 
/// The version in this clsss will always return an error.  Subclasses should override
/// this.
///
/// @return A pointer to a char array with an error message.  It should be NULL if there
/// is no error.

char* MenuPanel::errorInFields(void)
{
  static char* errString = (char*)"Unimplemented superclass method "
                                                              "MenuPanel::errorInFields.\n";
  return errString;
}


// =======================================================================================
/// @brief Interface for displaying the panel when it is supposed to be on screen.  
/// 
/// The version in this clsss will always return an error.  Subclasses should override
/// this with the specific buttons, fields, etc interface controls that they require to
/// get their state from the user.

void MenuPanel::display(void)
{
  err(-1, "Unimplemented superclass MenuPanel::diaplsy");
}


// =======================================================================================
/// @brief Interface for handling requests for some kind of panel action from the HTTP 
/// diagnostic server.  
/// 
/// Each panel's actions (buttons, fields, etc) can be simulated from the HTTP server
/// for scripting/testing purposes.  This interface defines a method for handling such
/// requests.  Note that this will *not* be called on the main thread, but rather one
/// of the HTTP server threads.  Accordingly, it should not do anything that is not
/// thread safe.  Usually it creates an Interface action which is queued for handling
/// on the main thread.
/// The version in this clsss will always return an error.  Subclasses should override
/// this and conform to the following interface.
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuPanel::handleHTTPRequest(HttpDebug* serv, char* path)
{
  err(-1, "Unimplemented superclass MenuPanel::handleHTTPRequest");
}


// =======================================================================================
/// @brief Method for setting the corner of menus in the corners of the window

void MenuPanel::setCorner(int& corner)
{
  const float DISTANCE = 10.0f;
  ImGuiIO& io = ImGui::GetIO();

  if (corner != -1)
   {
    ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE :
                             DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
   }
}


// =======================================================================================
/// @brief Method for setting menu in the center of the window

void MenuPanel::setCenter(void)
{
  ImGuiIO& io = ImGui::GetIO();
  ImVec2 window_pos = ImVec2(io.DisplaySize.x/2, io.DisplaySize.y/2);
  ImVec2 window_pos_pivot = ImVec2(0.5f, 0.5f);
  ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
}


// =======================================================================================
/// @brief Method to handle the details around creation of an InterfaceAction and 
/// associated error reporting.
///
/// Note this is generally called on an HTTP debug server thread, so don't do anything
/// that isn't thread safe in here.
/// @returns True if everything went well, false if there was an error.
/// @param serv The HttpDebug server instance to talk to.
/// @param actionType The ActionType of the InterfaceAction we are to create.
/// @param actionName The name of the actionType for logging purposes
/// @param functionName The name of the calling function for logging purposes
/// @param path The latter portion of the URL leading to this action creation.

bool MenuPanel::createAction(HttpDebug* serv, ActionType actionType, 
                                          char* actionName, char* functionName, char* path)
{
  InterfaceAction* action = new InterfaceAction(actionType, path);
  if(!action->valid)
   {
    LogRequestErrors("MenuInterface::%s couldn't create %s action.\n", 
                                                              functionName, actionName);
    return false;
   }
  scene->actions.push_back(action);
  httPrintf("OK\n")
  return true;
}


// =======================================================================================
