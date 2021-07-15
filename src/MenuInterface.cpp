// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Utility class for opening a window and setting up for drawing in using
// OpenGL commands.  The present implementation uses GLFW but the API should
// hide that.

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
// Constructor for a Window3D

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
// Function we call when something has gone wrong and we need to create an errorpanel

void  MenuInterface::createErrorPanel(const char* errString)
{
  
}


// =======================================================================================
// Transparent overlay that shows when we have clicked on an object that is now
// the basis for motion.

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
// Utility function to put a matrix into the current imgui window.

void displayImguiMatrix(const char* title, const mat4& m)
{
  ImGui::Text("%s\n", title);
  for(int i=0; i<4; i++)
    ImGui::Text("%.2f %.2f %.2f %.2f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
}


// =======================================================================================
// Top level function used only during startup when we need to interact with the user
// about something.

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
// Top level function to organize the rendering of all the imguiInterface stuff

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
// Function to tell HTTP interface what options the current menu has (especially for
// navigating tree selection menus.

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
// Function to handle entered parameters.

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
// Function to handle the parsing of selection actions (ie everything under /select comes
// here first.

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
// Function to handle the parsing of interface actions coming from HTTP (ie for test
// scripts).

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
/// @brief Handle an pseudo-interface event coming from the HTTP Debug interface.  We are
/// called from the equivalent function in Window3D to handle actions that are associated
/// with the menu system.
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
