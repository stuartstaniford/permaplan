// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// The job of this class is to provide a menu capable of routing us to task-specific
// panels for all the different kinds of things that can be insderted (height 
// measurements, building components, trees/plants, etc).

#include "MenuInsert.h"
#include "MenuBlockPanel.h"
#include "MenuShedPanel.h"
#include "MenuInterface.h"


// =======================================================================================
/// @brief The constructor is mostly a pass through to MenuPanel.
/// @param menu Pointer to the overall MenuInterface so we can call back into it. 
/// @param S A pointer to the scene.

MenuInsert::MenuInsert(MenuInterface* menu, Scene* S):
                                        MenuPanel(menu, S)
{
}


// =======================================================================================
/// @brief Destructor

MenuInsert::~MenuInsert(void)
{
}


// =======================================================================================
/// @brief Display the insert menu via Dear ImGui.
///
/// Calls all the various ImGui calls to actually put the menu together.

void MenuInsert::imGuiDisplay(void)
{
  if(!displayVisible)
    return;
  ImGui::Begin("Insert", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);
    
  if(ImGui::Button("Height"))
    insertHeight();
  if(ImGui::Button("Block"))
    insertBlock();
  if(ImGui::Button("Shed"))
    insertShed();
  if(ImGui::Button("Gable"))
    insertGable();
  if(ImGui::Button("Tree"))
    insertTree();

  ImGui::End();
}


// =======================================================================================
/// @brief Insert height button action (also from HTTP debug interface)

void MenuInsert::insertHeight(void)
{
  mainMenu->show_height_input_dialog = true;
  mainMenu->insertMenu = NULL;
  delete this;
}


// =======================================================================================
/// @brief Insert block button action (also from HTTP debug interface)

void MenuInsert::insertBlock(void)
{
  unless(mainMenu->blockPanel)
    mainMenu->blockPanel = new MenuBlockPanel(mainMenu, scene);
  mainMenu->insertMenu = NULL;
  delete this;
}


// =======================================================================================
/// @brief Insert block button action (also from HTTP debug interface)

void MenuInsert::insertShed(void)
{
  unless(mainMenu->shedPanel)
    mainMenu->shedPanel = new MenuShedPanel(mainMenu, scene);
  mainMenu->insertMenu = NULL;
  delete this;
}


// =======================================================================================
/// @brief Insert block button action (also from HTTP debug interface)

void MenuInsert::insertGable(void)
{
  //show_materials_menu = true;
  mainMenu->insertMenu = NULL;
  delete this;
}


// =======================================================================================
/// @brief Insert tree button action (also from HTTP debug interface)

void MenuInsert::insertTree(void)
{
  mainMenu->show_tree_menu = true;
  mainMenu->insertMenu = NULL;
  delete this;
}


// =======================================================================================
/// @brief Function to handle parsing insert menu related requests from the HTTP 
/// diagnostic server.  
/// 
/// Note that this will *not* be called on the main thread, but rather one
/// of the HTTP server threads.  
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuInsert::handleHTTPRequest(HttpDebug* serv, char* path)
{
  unless(displayVisible)
   {
    LogRequestErrors("MenuInsert::handleHTTPRequest called when no insert menu showing.\n");
    return false;
   }
  
  if(strncmp(path, "height", 6) == 0)
    return createAction(serv, InsertHeight, (char*)"InsertHeight", 
                                          (char*)"MenuInsert::handleHTTPRequest", path+6);

  if(strncmp(path, "block", 5) == 0)
    return createAction(serv, InsertBlock, (char*)"InsertBlock", 
                                          (char*)"MenuInsert::handleHTTPRequest", path+5);

  if(strncmp(path, "shed", 4) == 0)
    return createAction(serv, InsertShed, (char*)"InsertShed", 
                                          (char*)"MenuInsert::handleHTTPRequest", path+4);

  if(strncmp(path, "gable", 5) == 0)
    return createAction(serv, InsertGable, (char*)"InsertGable", 
                                          (char*)"MenuInsert::handleHTTPRequest", path+5);

  if(strncmp(path, "tree", 4) == 0)
    return createAction(serv, InsertTree, (char*)"InsertTree", 
                                          (char*)"MenuInsert::handleHTTPRequest", path+4);

  LogRequestErrors("MenuInsert::handleHTTPRequest unknown insert command %s\n", path);
  return false;
}


// =======================================================================================
/// @brief Interface for the method to be called in the main thread to process an 
/// insert menu related interface action.
/// 
/// Note the action will have originally arisen in the HTTP server and have been 
/// processed by MenuInsert::handleHTTPRequest).  
/// @returns The ActionType of the action we processed.
/// @param action The InterfaceAction that needs to be handled.

ActionType MenuInsert::processAction(InterfaceAction* action)
{
  switch(action->actionType)
   {
    case InsertBlock:
      LogPseudoActions("Insert block button press processed.\n");
      insertBlock();
      return InsertBlock;

    case InsertShed:
      LogPseudoActions("Insert shed button press processed.\n");
      insertShed();
      return InsertShed;

    case InsertGable:
      LogPseudoActions("Insert gable button press processed.\n");
      insertGable();
      return InsertGable;

    case InsertHeight:
      LogPseudoActions("Insert height button press processed.\n");
      insertHeight();
      return InsertHeight;       
        
    case InsertTree:
      LogPseudoActions("Insert tree button press processed.\n");
      insertTree();
      return InsertTree;

    default:
      return NoAction;
   }
}


// =======================================================================================
