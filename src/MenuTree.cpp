// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This is the menu that comes up when "Tree" is initially selected from the main 
// insert menu, and allows the user to select from amongst the genera that have already 
// been used in this scene.

#include "MenuTree.h"
#include "InterfaceMainSceneWin.h"
#include "MenuGenus.h"
#include "MenuAllTree.h"
#include "Species.h"
#include "HttpDebug.h"

// =======================================================================================
/// @brief Constructor

MenuTree::MenuTree(MenuInterface* menu, Scene* S):
                                        MenuPanel(menu, S)
{
}


// =======================================================================================
/// @brief Destructor

MenuTree::~MenuTree(void)
{
}


// =======================================================================================
/// @brief Display the tree menu via Dear ImGui.
///
/// Calls all the various ImGui calls to actually put the menu together.

void MenuTree::imGuiDisplay(void)
{
  if(!displayVisible)
    return;
      
  ImGui::Begin("Tree Genus", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);
  
  for(auto& iter: Species::genusList)
   {
    if(strcmp(iter.first.c_str(), "Nosuchgenus") == 0)
      continue;
    char genusOption[128];
    snprintf(genusOption, 128, "%s (%u)", iter.first.c_str(), iter.second);
    if(ImGui::Button(genusOption))
      treeMenuSelection(iter.first.c_str());
   }
  if(ImGui::Button("All Tree Selector"))
    treeMenuSelection((char*)"All Tree Selector");

  ImGui::End();
}


// =======================================================================================
/// @brief Handle one of the buttons being pressed in the tree menu.
///
/// Also called from Window3D::processPseudoAction to handle HTTP scripting.
/// @param genusString A C string with the name of the selected genus.  The special case
/// "All Tree Selector" invokes another set of menus to find a tree from any known to
/// permaplan.

void MenuTree::treeMenuSelection(const char* genusString)
{
  if(strcmp(genusString, "All Tree Selector") == 0)
    ((InterfaceMainSceneWin*)mainMenu)->allTreeMenu = new MenuAllTree(mainMenu, scene);
  else
    ((InterfaceMainSceneWin*)mainMenu)->genusMenu 
                                            = new MenuGenus(mainMenu, scene, genusString);
  
  ((InterfaceMainSceneWin*)mainMenu)->treeMenu = NULL;
  delete this;
  LogFlush();
}


// =======================================================================================
/// @brief Function to handle tree genus selection requests from the HTTP diagnostic 
/// server.  
/// 
/// Note that this will *not* be called on the main thread, but rather one
/// of the HTTP server threads.  
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuTree::handleHTTPRequest(HttpDebug* serv, char* path)
{
  unencode(path);
  
  if(!displayVisible)
   {
    LogRequestErrors("MenuTree::handleHTTPRequest with displayVisible false.\n");
    return false;    
   }
  
  if(strncmp(path, "All Tree Selector", 17) == 0 || Species::genusList.count(path) )
    return createAction(serv, SelectGenus, (char*)"SelectGenus", 
                                        (char*)"MenuTree::handleHTTPRequest", path);
  
  LogRequestErrors("MenuTree::handleHTTPRequest unknown selection %s\n", path);
  return false;
}


// =======================================================================================
/// @brief Function to handle requests from the HTTP diagnostic server to know what the 
/// currently available options are.  
/// 
/// Note that this will *not* be called on the main thread, but rather one
/// of the HTTP server threads.  
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuTree::handleOptionRequest(HttpDebug* serv, char* path)
{
  for(auto& iter: Species::genusList)
   {
    if(strcmp(iter.first.c_str(), "Nosuchgenus") == 0)
      continue;
    httPrintf("%s (%u)\n", iter.first.c_str(), iter.second);
   }
  httPrintf("All Tree Selector\n");
  return true;
}


// =======================================================================================
/// @brief Interface for the method to be called in the main thread to process a 
/// tree genus selection interface action.
/// 
/// Note the action will have originally arisen in the HTTP server and have been 
/// processed by MenuTree::handleHTTPRequest).  
/// @returns The ActionType of the action we processed.
/// @param action The InterfaceAction that needs to be handled.
/// @todo presently defaults to Strawbale all the time

ActionType MenuTree::processAction(InterfaceAction* action)
{
  const char* stringValue;
  switch(action->actionType)
   {
    case SelectGenus:
      stringValue = ((SpeciesPath*)(action->otherData))->getPath();
      LogPseudoActions("Genus selection of %s processed.\n", stringValue);
      treeMenuSelection(stringValue);
      return SelectGenus;

     default:
      return NoAction;
   }
}


// =======================================================================================
