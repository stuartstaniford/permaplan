// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class handles the UI panel for inserting a block.

#include "MenuBlockPanel.h"
#include "Material.h"
#include "MenuInterface.h"


// =======================================================================================
/// @brief Constructor
///
/// The constructor is mostly a pass through to MenuPanel.
/// @param menu Pointer to the overall MenuInterface so we can call back into it. 
/// @param S A pointer to the scene.

MenuBlockPanel::MenuBlockPanel(MenuInterface* menu, Scene* S):
                                      MenuPanel(menu, S)
{
  sizeBuf[0] = '\0';
  displayVisible = true;
}


// =======================================================================================
// Destructor

MenuBlockPanel::~MenuBlockPanel(void)
{
}


// =======================================================================================
/// @brief Provide the imGui interface for the BlockPanel.

void MenuBlockPanel::imGuiDisplay(void)
{
  if(!displayVisible)
    return;
  ImGui::Begin("Block Size and Material", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::InputText("", sizeBuf, 8, ImGuiInputTextFlags_CharsDecimal);

  const MaterialList& materials = MaterialList::getMaterials();
  
  for(auto& iter: materials)
    if(ImGui::Button(iter.first.c_str()))
     {
      size = atof(sizeBuf);
      sizeBuf[0] = '\0';
      blockEntered(size, iter.first);
      ImGui::End();
      return;
     }
  ImGui::End();
}


// =======================================================================================
/// @brief Interface for the method to be called in the main thread to process a 
/// block panel related interface action.
/// 
/// Note the action will have originally arisen in the HTTP server and have been 
/// processed by handleHTTPRequest.  
/// @returns The ActionType of the action we processed.
/// @param action The InterfaceAction that needs to be handled.
/// @todo presently defaults to Strawbale all the time

ActionType MenuBlockPanel::processAction(InterfaceAction* action)
{
  switch(action->actionType)
   {
    case BlockEntered:
      LogPseudoActions("Block entered size %.1f, material Strawbale.\n", action->data[0]);
      blockEntered(action->data[0], "StrawBale");
      return BlockEntered;
   
    default:
      return NoAction;
   }
}


// =======================================================================================
/// @brief Function to handle block entry requests from the HTTP diagnostic server.  
/// 
/// Note that this will *not* be called on the main thread, but rather one
/// of the HTTP server threads.  
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuBlockPanel::handleHTTPRequest(HttpDebug* serv, char* path)
{
  return createAction(serv, BlockEntered, (char*)"BlockEntered", 
                                (char*)"MenuBlockPanel::handleHTTPRequest", path);
}


// =======================================================================================
/// @brief Block entered button action (also from HTTP debug interface)
/// @param blockSize The size of the block to be created (in SpaceUnits).
/// @param matName The name of the material the block is to be made of.

void MenuBlockPanel::blockEntered(float blockSize, const std::string& matName)
{
  const MaterialList& materials = MaterialList::getMaterials();
  auto iter = materials.find(matName);
  scene->insertVisibleObject((char*)"Block", blockSize, scene->lastDoubleClick, iter->second);
  LogMaterialSelections("Material %s selected for block, carbon density %.2f.\n",
                        matName, iter->second->carbonDensity);
  mainMenu->blockPanel = NULL;
  delete this;
}


// =======================================================================================
