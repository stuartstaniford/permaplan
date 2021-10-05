// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This panel appears on clicking "Height" from the insert menu, but should also be
// available from the HTTP interface, and in particular it's anticipated in future that
// the most used entry point to this interface will be via a phone app using GPS.

#include "MenuHeightPanel.h"
#include "InterfaceMainSceneWin.h"
#include "Scene.h"


// =======================================================================================
/// @brief Constructor
///
/// @param menu Pointer to the overall MenuInterface so we can call back into it. 
/// @param S A pointer to the scene.

MenuHeightPanel::MenuHeightPanel(MenuInterface* menu, Scene* S):
                                                MenuPanel(menu, S)
{
  // clear the buffers
  heightBuf[0] = '\0'; 
}


// =======================================================================================
/// @brief Destructor

MenuHeightPanel::~MenuHeightPanel(void)
{
}


// =======================================================================================
///@brief Users wants to insert a height, and we need to know more
/// @todo need to be able to enter a label

void MenuHeightPanel::imGuiDisplay(void)
{
  if(!displayVisible)
    return;
  ImGui::Begin("Insert Height", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);
  
  ImGui::InputText("", heightBuf, 8, ImGuiInputTextFlags_CharsDecimal);
  
  if(ImGui::Button("Height"))
   {
    float z;
    z = atof(heightBuf);
    heightEntered(z);
    heightBuf[0] = '\0';
   }

  ImGui::End();
}


// =======================================================================================
/// @brief Height entered button action (also from HTTP debug interface)
///
/// @param z The altitude to enter

void MenuHeightPanel::heightEntered(float z)
{
  scene->lastDoubleClick[2] = z;
  scene->newLandHeight(scene->lastDoubleClick, NULL); 
  ((InterfaceMainSceneWin*)mainMenu)->heightPanel = NULL;
  delete this;
}

// =======================================================================================
/// @brief Function to handle height entry requests from the HTTP diagnostic server.  
/// 
/// Note that this will *not* be called on the main thread, but rather one
/// of the HTTP server threads.  
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuHeightPanel::handleHTTPRequest(HttpDebug* serv, char* path)
{
  if(!displayVisible)
   {
    LogRequestErrors("MenuHeightPanel::handleHTTPRequest with displayVisible false.\n");
    return false;    
   }
  return createAction(serv, HeightEntered, (char*)"HeightEntered", 
                                      (char*)"MenuHeightPanel::handleHTTPRequest", path);  
  return false;
}


// =======================================================================================
/// @brief Process the entry of a new height in the main thread.
/// 
/// Note the action will have originally arisen in the HTTP server and have been 
/// processed by MenuHeightPanel::handleHTTPRequest).  
/// @returns The ActionType of the action we processed.
/// @param action The InterfaceAction that needs to be handled.

ActionType MenuHeightPanel::processAction(InterfaceAction* action)
{
  switch(action->actionType)
   {
    case SelectGenus:
      LogPseudoActions("Height of %.1f entered.\n", action->data[0]);
      heightEntered(action->data[0]);
      return HeightEntered;

     default:
      return NoAction;
   }
}


// =======================================================================================
