// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class ..... is a template for real classes

#include "MenuAllTree.h"
#include "DynamicallyTypable.h"
#include "Species.h"
#include "RegionList.h"
#include "InterfaceMainSceneWin.h"
#include "Tree.h"
#include "Scene.h"
#include "HttpDebug.h"


// =======================================================================================
/// @brief Constructor
///
/// The constructor is mostly a pass through to MenuPanel.  But we also initialize
/// currentList, which is our pointer to which list we are currently displaying in menu
/// form.
/// @param menu Pointer to the overall MenuInterface so we can call back into it. 
/// @param S A pointer to the scene.

MenuAllTree::MenuAllTree(MenuInterface* menu, Scene* S):
                                      MenuPanel(menu, S)
{
  RegionList& regionRoot = RegionList::getRoot();
  currentList = &regionRoot;
}


// =======================================================================================
/// @brief Destructor

MenuAllTree::~MenuAllTree(void)
{
}


// =======================================================================================
/// @brief Display the menu based on currentList via Dear ImGui.
///
/// Calls all the various ImGui calls to actually put the menu together.

using namespace rapidjson;

void MenuAllTree::imGuiDisplay(void)
{
  if(!displayVisible)
    return;
    
  ImGui::Begin("Tree Regions", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);
  for (auto iter : *currentList) 
   {
    DynamicType dtype = iter.second->getDynamicType();
    
    if(dtype == TypeRegionList)
     {
      RegionList* r = (RegionList*)iter.second;
      if(r->size() == 0)
        continue; // no point in displaying empty lists.
     }
    
    // If we get this far, there is a button to display
    if(ImGui::Button(iter.first.c_str()))
     {
      allTreeSelection(iter.first.c_str(), iter.second);
      break;
     }
   }
  ImGui::End();
}


// =======================================================================================
/// @brief Process a selection in the all tree selector, that might be coming from either 
/// the GUI or from the HTTP interface.
/// @param name The name of this menu option.
/// @param value A dynamically typable value which can point to either a RegionList of 
/// other things, or a specific Species to be selected.

void MenuAllTree::allTreeSelection(const char* name, DynamicallyTypable* value)
{
  DynamicType dtype = value->getDynamicType();
  
  if(dtype == TypeRegionList)
   {
    // We need to recurse down the list tree
    if(((RegionList*)value)->size() == 0)
      return; // can't process an empty list.
    currentList = (RegionList*)value;
    return;
   }
  else if(dtype == TypeSpecies || dtype == TypeSpeciesPath) 
   {
    // species selection has been made
    if(dtype == TypeSpeciesPath)
     {
      // need to load it.
      SpeciesPath* spath = (SpeciesPath*)value;
      Species* species = Species::getSpeciesByPath(spath->getPath());
      delete spath;
      if(!species)
        err(-1, "Couldn't find species.\n");
      (*currentList)[name] = species;
      dtype = TypeSpecies;
     }
    if(dtype == TypeSpecies)
     {
      Species* S = (Species*)((*currentList)[name]);
      LogTreeSelections("Tree %s %s inserted at [%f, %f].\n", 
                        S->genusName, S->speciesName,
                        scene->lastDoubleClick[0], scene->lastDoubleClick[1]);
      Tree* tree = new Tree(S, scene->lastDoubleClick, 1.0f, scene->getSimYear());
      scene->insertVisualObject(tree);
      ((InterfaceMainSceneWin*)mainMenu)->allTreeMenu = NULL;
      delete this;
     }
   }
}


// =======================================================================================
/// @brief Function to handle selecting a tree from the HTTP diagnostic server.  
/// 
/// Note that this will *not* be called on the main thread, but rather one
/// of the HTTP server threads.  
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel 
/// (the path required to navigate to us is hidden from us because it's not our concern.

bool MenuAllTree::handleHTTPRequest(HttpDebug* serv, char* path)
{
  unencode(path);
  
  if(strlen(path) > 128)
   {
    // Weird long paths get cut off here.  Otherwise path checking has to be done
    // downstream from us.
    LogRequestErrors("MenuInterface::HTTPAPiAllTreeSelector doesn't like long path.\n");
    return false;        
   }
  
  return createAction(serv, AllTreeSelection, (char*)"AllTreeSelection", 
                                          (char*)"MenuAllTree::handleHTTPRequest", path);
}


// =======================================================================================
/// @brief Called in the main thread to process a selection of the all tree selector 
/// coming from the HTTP interface.
/// 
/// Note the action will have originally arisen in the HTTP server and have been 
/// processed by handleHTTPRequest).  
/// @returns The ActionType of the action we processed.
/// @param action The InterfaceAction that needs to be handled.

ActionType MenuAllTree::processAction(InterfaceAction* action)
{
  // Warning stringValue is unprocessed except for being <=128
  const char* stringValue = ((SpeciesPath*)(action->otherData))->getPath();
  LogPseudoActions("All tree selection %s processed.\n", stringValue);
  unless(currentList && currentList->count(stringValue))
   {
    LogRequestErrors("MenuAllTree::processAction invalid action %s.\n", stringValue);
    return NoAction;
   }
  allTreeSelection(stringValue, (*currentList)[stringValue]);
  return AllTreeSelection;
}


// =======================================================================================
/// @brief Function to tell HTTP interface what options the current menu has (especially 
/// for navigating tree selection menus.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel 
/// (the path required to navigate to us is hidden from us because it's not our concern.

bool MenuAllTree::handleOptionRequest(HttpDebug* serv, char* path)
{
  if(currentList)
   {     
    for (auto iter : *currentList) 
     {
      DynamicType dtype = iter.second->getDynamicType();
      if(dtype == TypeRegionList)
       {
        RegionList* r = (RegionList*)iter.second;
        if(r->size() == 0)
          continue; // no point in displaying empty lists.
       }
      httPrintf("%s\n", iter.first.c_str());
     }
    return true;
   }
  else
   {
    LogRequestErrors("MenuAllTree::handleOptionRequest with NULL currentList.\n");
    return false;
   }
 }
  
  
// =======================================================================================
