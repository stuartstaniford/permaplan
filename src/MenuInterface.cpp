// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// This class is the master class in charge of the whole menu interface - both the 
// ImGui windows shown on screen as well as the HTTP interface to the various options
// that is used for scripting/testing.  It is where all the pointers live that point
// to instances of all the different individuals menus and panels 

#include "MenuInterface.h"
#include "Material.h"
#include "Species.h"
#include "Window3D.h"
#include "MenuShedPanel.h"
#include "MenuBlockPanel.h"
#include "MenuFocusOverlay.h"
#include "MenuSimulationPanel.h"
#include "MenuInsert.h"
#include "MenuGenus.h"
#include "MenuTree.h"
#include "loadFileToBuf.h"
#include "RegionList.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


// =======================================================================================
/// @brief Constructor
///
/// Does the ImGui initialization, and NULL initializes all the individual panel/menu 
/// pointers.
/// @param W A reference to the Window3D with which this instance is associated.

MenuInterface::MenuInterface(Window3D& W):
                        MenuPanel(this, NULL), // scene will be set later
                        show_lock_overlay(false),
                        show_init_panel(false),
                        win3D(W),
                        shedPanel(NULL),
                        blockPanel(NULL),
                        heightPanel(NULL),
                        simulationPanel(NULL),
                        focusOverlay(NULL),
                        insertMenu(NULL),
                        treeMenu(NULL),
                        genusMenu(NULL),
                        allTreeMenu(NULL)
#ifdef SHOW_DEMO_WINDOW
                        , show_demo_window(true)
#endif
{
  // Dear ImGui initialization
  // Setup context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(win3D.window, true);
  ImGui_ImplOpenGL3_Init("#version 410");
  // Setup Dear ImGui style
  ImGui::StyleColorsClassic();
}


// =======================================================================================
/// @brief Ssomething has gone wrong and we need to create an errorpanel.
/// @param errString - a C string describing the problem.
/// @todo This is currently a stub with no implementation.

void  MenuInterface::createErrorPanel(const char* errString)
{
  
}


// =======================================================================================
/// @brief Transparent overlay that shows when we have clicked on an object that is now
/// the basis for motion.
/// @todo This has not been moved to it's own MenuPanel subclass because it's somewhat
/// flawed functionality.  It needs to either be fixed (and moved to a new class) or
/// removed.

void MenuInterface::imguiLockOverlay(void)
{
  if(!show_lock_overlay)
    return;
  static int corner = 3;
  
  setCorner(corner);
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
  if (corner != -1)
    window_flags |= ImGuiWindowFlags_NoMove;
  if (ImGui::Begin("Lock Overlay", &show_lock_overlay, window_flags))
   {
    if(scene->lockObject)
     {
      ImGui::Text("Object Type: %s\n", scene->lockObject->objectName());
      ImGui::Text("Coords: %.1f' east, %.1f' north\nAltitude: %.1f'\n",
                  scene->lockObjectLocation[0], scene->lockObjectLocation[1],
                  scene->lockObjectLocation[2]);
     }
    else
      ImGui::Text("No focus object.\n");

    ImGui::Separator();
   }
  ImGui::End();
}


// =======================================================================================
/// @brief Query the user about something during startup.
///
/// This function is a special case used only during startup when we need to interact 
/// with the user about something (often something that has gone wrong).  It is only
/// called from Window3D::initPanel, which has it's own special rendering loop because
/// this is for use before the main rendering loop has started.
/// @returns An integer which is the selection the user made from the supplied 
/// responses array.
/// @param question A C string with the question being posed to the user.
/// @param responses An array of C strings with possible answers which will be put on
/// different buttons the user can press.
/// @param nResponses The number of things in the responses array.

int  MenuInterface::initPanel(char* question, char** responses, int nResponses)
{
  unless(show_init_panel)
    return -1;
  
  // Initialization of window
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Format and position
  setCenter();
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

  // Contents of window
  if (ImGui::Begin("Dialog Box", &show_init_panel, window_flags))
   {
    ImGui::Text("%s", question);
    ImGui::Separator();
    
    for(int i=0; i < nResponses; i++)
     {
      //fprintf(stderr, "Creating button %d\n",i);
      if(ImGui::Button(responses[i]))
       {
        show_init_panel = false;
        //fprintf(stderr, "Selected button %d\n",i);
        return i;
       }
      ImGui::SameLine();
     }
   }
  ImGui::End();

  // Render dear imgui into screen
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  return -1;  
}


// =======================================================================================
/// @brief Top level function to render all active menus/panels.
///
/// This is the function called from the main rendering loop (Window3D::loop) to display
/// any menus or panels that should be active (which is determined by their being a
/// non-NULL pointer).

void MenuInterface::imguiInterface(void)
{
  // Imgui preamble stuff
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Always visible panels/menus
  unless(simulationPanel)
    simulationPanel = new MenuSimulationPanel(this, scene);
  simulationPanel->imGuiDisplay();
  unless(focusOverlay)
    focusOverlay = new MenuFocusOverlay(this, scene, win3D);
  focusOverlay->imGuiDisplay();
  
  // Panels/menus that may or may not be visible
  if(shedPanel)
    shedPanel->imGuiDisplay();
  if(blockPanel)
    blockPanel->imGuiDisplay();
  if(insertMenu)
    insertMenu->imGuiDisplay();
  if(treeMenu)
    treeMenu->imGuiDisplay();
  if(genusMenu)
    genusMenu->imGuiDisplay();
  if(heightPanel)
    heightPanel->imGuiDisplay();
  if(allTreeMenu)
    allTreeMenu->imGuiDisplay();
    
  // Unconverted menus  
  imguiLockOverlay();

#ifdef SHOW_DEMO_WINDOW
  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);
#endif
  
  // Render dear imgui into screen
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


// =======================================================================================
/// @brief Top level function to parse interface requests from the HTTP server.
/// 
/// This function to is to handle the parsing of menu interface actions coming from HTTP 
/// (ie for test scripts).  Everything with a URL path starting with /menu/ comes here.
/// We figure out the next component in the path and then route it to the right place.
/// Most requests will end up at the handleHTTPRequest method of some subclass of
/// MenuPanel.
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuInterface::HTTPAPi(HttpDebug* serv, char* path)
{  
  if(strncmp(path, "simulate/", 9)== 0)
   {
    if(simulationPanel)
      return simulationPanel->handleHTTPRequest(serv, path+9);
    else
      return false;
   }

  if(strncmp(path, "insert/", 7)== 0)
   {
    if(insertMenu)
      return insertMenu->handleHTTPRequest(serv, path+7);
    else
     {
      LogRequestErrors("Insert request for %s when insert menu not showing.", path+7);
      return false;
     }
   }

  if(strncmp(path, "enter/", 6)== 0)
    return HTTPAPiEnter(serv, path+6);

  if(strncmp(path, "options/", 8)== 0)
    return HTTPAPiOptions(serv, path+8);

  if(strncmp(path, "select/", 7)== 0)
    return HTTPAPiSelections(serv, path+7);
  
  LogRequestErrors("MenuInterface::HTTPAPi unknown directive %s\n", path);
  return false;
}


// =======================================================================================
/// @brief Report on the options of the on-screen menu
/// 
/// Function to tell HTTP interface what options the current menu has (especially for
/// navigating tree selection menus.  Ie this is to route requests under /menu/options/,
/// which will typically end up at the handleOptionRequest method of a MenuPanel 
/// subclass.
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.
/// @todo Many menus have not implemented this yet.

bool MenuInterface::HTTPAPiOptions(HttpDebug* serv, char* path)
{
  if(treeMenu)
    return treeMenu->handleOptionRequest(serv, path);
  else if(allTreeMenu)
    return allTreeMenu->handleOptionRequest(serv, path);
  else
   {
    httPrintf("OK\n");
    //err(-1, "MenuInterface::HTTPAPiOptions called on unimplemented menu.");
   }
  return true;
}


// =======================================================================================
/// @brief Handle entered parameters.
/// 
/// This Function to handle entered parameters that come in under /menu/enter and route
/// them on in the correct direction.
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuInterface::HTTPAPiEnter(HttpDebug* serv, char* path)
{  
  if(heightPanel && strncmp(path, "height/", 7) == 0)
    return heightPanel->handleHTTPRequest(serv, path+7);

  if(blockPanel && strncmp(path, "block/", 6) == 0)
    return blockPanel->handleHTTPRequest(serv, path+6);

  LogRequestErrors("MenuInterface::HTTPAPiEnter incorrect enter command %s\n", path);
  return false;
}


// =======================================================================================
/// @brief Handle selection options.
/// 
/// Function to handle the parsing of selection actions (ie everything under /menu/select 
/// comes here first and we route them on in the correct direction.
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuInterface::HTTPAPiSelections(HttpDebug* serv, char* path)
{  
  if(strncmp(path, "genus/", 6)== 0)
   {
    if(treeMenu)
     return treeMenu->handleHTTPRequest(serv, path+6);
    else
     {
      LogRequestErrors("Selection %s from tree menu that isn't showing", path+6);
      return false;
     }
   }
  
  if(strncmp(path, "alltree/", 8)== 0)
   {
    if(allTreeMenu)
      return allTreeMenu->handleHTTPRequest(serv, path+8);
    else
     {
      LogRequestErrors("MenuInterface::HTTPAPiSelections with no all tree menu.\n");
      return false;    
     }
   }
  
  LogRequestErrors("MenuInterface::HTTPAPiSelections unknown directive %s\n", path);
  return false;
}


// =======================================================================================
/// @brief Handle a pseudo-interface event coming from the HTTP Debug interface.  We are
/// called from the equivalent function in Window3D to handle actions that are associated
/// with the menu system.  Generally we are routing the request on to the processAction
/// method for the appropriate subclass of MenuPanel.
///
/// ActionTypes are listed in alphabetical order
/// @returns The ActionType of the action we processed.
/// @param action The InterfaceAction that needs to be handled.
/// @todo Possibly this should be handled via a std::map<ActionType, MenuPanel*>

ActionType MenuInterface::processAction(InterfaceAction* action)
{  
  switch(action->actionType)
   {
     case AllTreeSelection:
      if(allTreeMenu)
        return allTreeMenu->processAction(action);
      else
        return NoAction;

     case BlockEntered:
      if(blockPanel)
        return blockPanel->processAction(action);
      else
        return NoAction;

    case HeightEntered:
      if(heightPanel)
        return heightPanel->processAction(action);
      else
       {
        LogRequestErrors("Height Entered action when height panel not showing.\n");
        return NoAction;
       }

    // Insert menu actions get passed to the insert menu
    case InsertBlock:
    case InsertShed:
    case InsertGable:
    case InsertHeight:       
    case InsertTree:
      if(insertMenu)
        return insertMenu->processAction(action);
      else
       {
        LogRequestErrors("Insert action when insert menu not showing.\n");
        return NoAction;
       }

    // Tree selection menu
    case SelectGenus:
      if(treeMenu)
        return treeMenu->processAction(action);
      else
        return NoAction;
            
    // Simulation panel options handled over there.  
    case SimulatePause:
    case SimulateReset:
    case SimulateStart:
    case SimulateSpring:
    case SimulateSummer:
    case SimulateFall:
    case SimulateWinter:
      if(simulationPanel)
        return simulationPanel->processAction(action);
      else
        return NoAction;

    default:
      LogRequestErrors("Unhandled action type in MenuInterface::processAction %d\n", 
                                                            action->actionType);
      return NoAction;
   }
}


// =======================================================================================
