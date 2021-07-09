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
#include "loadFileToBuf.h"
#include "RegionList.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


// =======================================================================================
// Constructor for a Window3D

MenuInterface::MenuInterface(GLFWwindow* window, Window3D& W):
                        MenuPanel(this),
                        scene(NULL),
                        show_insert_menu(false),
                        show_lock_overlay(false),
                        show_init_panel(false),
                        win3D(W),
                        show_height_input_dialog(false),
                        show_tree_menu(false),
                        genusSelected(NULL),
                        show_focus_overlay(true),
                        show_simulation_controller(true),
                        all_tree_selector(false),
                        shedPanel(NULL),
                        blockPanel(NULL)
#ifdef SHOW_DEMO_WINDOW
                        , show_demo_window(true)
#endif
{
  // Dear ImGui initialization
  // Setup context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410");
  // Setup Dear ImGui style
  ImGui::StyleColorsClassic();

  // clear the buffers
  heightBuf[0] = '\0'; 
}


// =======================================================================================
// Function we call when something has gone wrong and we need to create an errorpanel

void  MenuInterface::createErrorPanel(const char* errString)
{
  
}


// =======================================================================================
// Users wants to insert a height, and we need to know more

void MenuInterface::imguiHeightInputDialog(void)
{
  if(!show_height_input_dialog)
    return;
  ImGui::Begin("Insert Height", &show_insert_menu, ImGuiWindowFlags_AlwaysAutoResize);
  
  ImGui::InputText("", heightBuf, 8, ImGuiInputTextFlags_CharsDecimal);
  
  if(ImGui::Button("Height"))
   {
    float z;
    z = atof(heightBuf);
    heightEnteredButton(z);
    heightBuf[0] = '\0';
   }

  ImGui::End();
}


// =======================================================================================
// Height entered button action (also from HTTP debug interface)

void MenuInterface::heightEnteredButton(float z)
{
  scene->lastDoubleClick[2] = z;
  scene->newLandHeight(scene->lastDoubleClick, NULL); //XX need to be able to enter a label
  show_height_input_dialog = false;
}


// =======================================================================================
// Insert height button action (also from HTTP debug interface)

void MenuInterface::insertHeightButton(void)
{
  show_height_input_dialog = true;
  show_insert_menu = false;
}


// =======================================================================================
// Insert block button action (also from HTTP debug interface)

void MenuInterface::insertBlockButton(void)
{
  show_insert_menu = false;
  unless(blockPanel)
    blockPanel = new MenuBlockPanel(this);
  blockPanel->displayVisible = true;
}


// =======================================================================================
// Insert block button action (also from HTTP debug interface)

void MenuInterface::insertShedButton(void)
{
  show_insert_menu = false;
  unless(shedPanel)
    shedPanel = new MenuShedPanel(this);
  shedPanel->displayVisible = true;
}


// =======================================================================================
// Insert block button action (also from HTTP debug interface)

void MenuInterface::insertGableButton(void)
{
  show_insert_menu = false;
  //show_materials_menu = true;
}


// =======================================================================================
// Insert tree button action (also from HTTP debug interface)

void MenuInterface::insertTreeButton(void)
{
  show_insert_menu = false;
  show_tree_menu = true;
}


// =======================================================================================
// The floating menu to insert stuff at the focus point

void MenuInterface::imguiInsertMenu(void)
{
  if(!show_insert_menu)
    return;
  ImGui::Begin("Insert", &show_insert_menu, ImGuiWindowFlags_AlwaysAutoResize);
    
  if(ImGui::Button("Height"))
    insertHeightButton();
  if(ImGui::Button("Block"))
    insertBlockButton();
  if(ImGui::Button("Shed"))
    insertShedButton();
  if(ImGui::Button("Gable"))
    insertGableButton();
  if(ImGui::Button("Tree"))
    insertTreeButton();

  ImGui::End();
}


// =======================================================================================
// Block entered button action (also from HTTP debug interface)

void MenuInterface::blockEnteredButton(float blockSize, const std::string& matName)
{
  const MaterialList& materials = MaterialList::getMaterials();
  auto iter = materials.find(matName);
  scene->insertVisibleObject((char*)"Block", blockSize, scene->lastDoubleClick, iter->second);
  LogMaterialSelections("Material %s selected for block, carbon density %.2f.\n",
                        matName, iter->second->carbonDensity);
  blockPanel = NULL;
}


// =======================================================================================
// The floating menu to select a particular species to insert after a genus has been
// selected in imguiTreeMenu.  Also gets the tree age at planting time.

void MenuInterface::imguiGenusMenu(void)
{
  unless(show_tree_menu && genusSelected)
    return;

  ImGui::Begin("Tree Species", &show_tree_menu, ImGuiWindowFlags_AlwaysAutoResize);

  // Age field
  ImGui::Text("Tree age at planting: ");
  ImGui::SameLine();
  ImGui::InputText("1.0", heightBuf, 5, ImGuiInputTextFlags_CharsDecimal);

  //Species selector
  ImGui::Text("Species: ");
  for(auto& iter: *(Species::genusSpeciesList[genusSelected]))
   {
    if(strcmp(iter.first.c_str(), "nosuchspecies") == 0)
      continue;
    ImGui::SameLine();
    if(ImGui::Button(iter.first.c_str()))
     {
      Species* S = iter.second;
      genusSelected = NULL;
      show_tree_menu = false;
      // XX we are not doing any sanity checking on the tree age here.  Need to make sure
      // it's numerical and non-negative.  Also need a default (eg 1.0f years)
      LogTreeSelections("Tree %s %s (age %.1f years) inserted at [%f, %f].\n",
                        S->genusName, S->speciesName, atof(heightBuf),
                                          scene->lastDoubleClick[0], scene->lastDoubleClick[1]);
      scene->insertTree(S, scene->lastDoubleClick, atof(heightBuf));
     }
   }
  ImGui::End();
}


// =======================================================================================
// The floating menu to select a particular tree to insert (by latin/scientific name)

void MenuInterface::imguiTreeMenu(void)
{
  if(!show_tree_menu)
    return;
  
  if(genusSelected)
    return imguiGenusMenu();
    
  ImGui::Begin("Tree Genus", &show_tree_menu, ImGuiWindowFlags_AlwaysAutoResize);
  
  for(auto& iter: Species::genusList)
   {
    if(strcmp(iter.first.c_str(), "Nosuchgenus") == 0)
      continue;
    char genusOption[128];
    snprintf(genusOption, 128, "%s (%u)", iter.first.c_str(), iter.second);
    if(ImGui::Button(genusOption))
      imguiTreeMenuButtonPressed(iter.first.c_str());
   }
  if(ImGui::Button("All Tree Selector"))
    imguiTreeMenuButtonPressed((char*)"All Tree Selector");

  ImGui::End();
}


// =======================================================================================
// Function to handle one of the buttons being pressed in MenuInterface::imguiTreeMenu.
// Also called from Window3D::processPseudoAction to handle HTTP scripting.

void MenuInterface::imguiTreeMenuButtonPressed(const char* genusString)
{
  if(strcmp(genusString, "All Tree Selector") == 0)
   {
    genusSelected = NULL;
    show_tree_menu = false;
    all_tree_selector = true;
   }
  else
    genusSelected = genusString;
  LogFlush();
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
// This menu is the entry point into a set of menu's (based on JSON files) that ultimately
// aspire to be able to select any tree species in the world.

using namespace rapidjson;

RegionList* currentList = NULL; // Effectively protected by the scene lock.

void MenuInterface::imguiAllTreeSelector(void)
{
  if(!all_tree_selector)
    return;
  
  unless(currentList)
   {
    RegionList& regionRoot = RegionList::getRoot();
    currentList = &regionRoot;
   }
  
  ImGui::Begin("Tree Regions", &all_tree_selector, ImGuiWindowFlags_AlwaysAutoResize);
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
      allTreeSelectorButton(iter.first.c_str(), iter.second);
     
   }
  ImGui::End();
}


// =======================================================================================
// Process a selection in the all_tree_selector, that might be coming from either the
// GUI or from the HTTP interface.

void MenuInterface::allTreeSelectorButton(const char* name, DynamicallyTypable* value)
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
        return;
      (*currentList)[name] = species;
      dtype = TypeSpecies;
     }
    if(dtype == TypeSpecies)
     {
      Species* S = (Species*)((*currentList)[name]);
      all_tree_selector = false;
      currentList = NULL;
      LogTreeSelections("Tree %s %s inserted at [%f, %f].\n", 
                        S->genusName, S->speciesName,
                        scene->lastDoubleClick[0], scene->lastDoubleClick[1]);
      scene->insertTree(S, scene->lastDoubleClick, 1.0f);
     }
   }
}


// =======================================================================================
// Called in the main thread to process a selection of the all tree selector coming
// from the HTTP interface.

void MenuInterface::allTreeSelectorPseudoAction(const char* optionName)
{
  unless(all_tree_selector && currentList && currentList->count(optionName))
   {
    LogRequestErrors("AllTreeSelectorPseudoAction invalid action %s.\n", optionName);
    LogFlush();
    return;
   }
  allTreeSelectorButton(optionName, (*currentList)[optionName]);
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
// Stuff that may or may not be turned on in the overlay window concerning the mouse
// location.

void MenuInterface::mouseOverlayDisplays(vec3 mouseSceneLoc)
{
  //ImGui::Text("Mouse: %.1f E, %.1f N %.1f altitude\n",
  //            mouseSceneLoc[0], mouseSceneLoc[1], mouseSceneLoc[2]);
  
  // Shows location of mouse in window co-ordinates
  /*ImGui::Text("Raw mouse: %.1f/%d, %.1f/%d\n", lastMouseX, width, lastMouseY, height);
  ImGui::Text("Clip mouse: %.3f [-1,1], %.3f [-1,1]\n",
              lastMouseX/width*2.0f-1.0f, 1.0f - lastMouseY/height*2.0f);
  */
  //Mouse Ray
  /*ImGui::Text("Mouse in world space: %.3f, %.3f, %.3f'\n", scene->lastMouseLocation[0],
              scene->lastMouseLocation[1], scene->lastMouseLocation[2]);
  ImGui::Text("Mouse direction: %.3f, %.3f, %.3f'\n", scene->lastMouseDirection[0],
              scene->lastMouseDirection[1], scene->lastMouseDirection[2]);
  */
  // Projection matrix
  //displayImguiMatrix("Projection Matrix:", scene->camera.projection);
  
  // View matrix
  //displayImguiMatrix("View Matrix:", scene->camera.view);
  
  // Model matrix
  //displayImguiMatrix("Model Matrix:", scene->model);
  
  // Invert matrix
  //displayImguiMatrix("Invert Matrix:", scene->invert);
}


// =======================================================================================
// Transparent overlay that displays information about the thing currently in focus

void MenuInterface::imguiFocusOverlay(void)
{
  if(!show_focus_overlay)
    return;
  static int corner = 0;
  
  setCorner(corner);
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
  if (corner != -1)
    window_flags |= ImGuiWindowFlags_NoMove;
  if (ImGui::Begin("Focus Overlay", &show_focus_overlay, window_flags))
   {
    //scene->findCameraObject(camF);
    scene->focusObject = scene->findObjectFromWindowCoords(scene->focusObjectLocation,
                    win3D.lastMouseX/win3D.width*2.0f-1.0f,
                    1.0f - win3D.lastMouseY/win3D.height*2.0f);
    if(scene->focusObject)
     {
      ImGui::Text("Object Type: %s\n", scene->focusObject->objectName());
      ImGui::Text("Coords: %.1f' east, %.1f' north\nAltitude: %.1f'\n",
                  scene->focusObjectLocation[0], scene->focusObjectLocation[1],
                  scene->focusObjectLocation[2]);
      mouseOverlayDisplays(scene->focusObjectLocation);
     }
    else
      ImGui::Text("Mouse out of scene.\n");

    ImGui::Text("Camera Height: %.1f'\n", scene->findCameraHeight());
    ImGui::Text("Frames/Sec: %.0f\n", 1.0f/win3D.frameTimeAvg);
    ImGui::Separator();
   }
  ImGui::End();
}


// =======================================================================================
// Transparent overlay that displays information about the thing currently in focus

void MenuInterface::imguiSimulationController(void)
{
  if(!show_simulation_controller)
    return;
  static int corner = 1;
  
  // Basic window setup
  setCorner(corner);
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
  if (corner != -1)
    window_flags |= ImGuiWindowFlags_NoMove;
  if (ImGui::Begin("Simulation Control", &show_simulation_controller, window_flags))
   {
    // XX Imgui not displaying Unicode symbols:
    // Check https://en.wikipedia.org/wiki/Media_control_symbols and
    // https://www.branah.com/unicode-converter
    // Play is U+25B6, (\xe2\x96\xb6 in UTF8)
    // Pause is U+23F8, (\xe2\x8f\xb8 in UTF8)
    // Skip to start is U+23EE, (\xe2\x8f\xae in UTF8)
    ImGui::Separator();
    ImGui::Text("Simulation Control");
    
    // Simulation controls
    if(ImGui::Button("Simulate"))
     {
      LogSimulationControls("Simulate Button \xe2\x96\xb6 pressed.\n");
      scene->startSimulation();
     }
    ImGui::SameLine();
    if(ImGui::Button("Pause"))
     {
      LogSimulationControls("Pause Button \xe2\x8f\xb8 pressed.\n");
      scene->pauseSimulation();
     }
    ImGui::SameLine();
    if(ImGui::Button("Reset"))
     {
      LogSimulationControls("Reset Button \xe2\x8f\xae pressed.\n");
      scene->resetSimulation();
     }
    
    // Auxiliary status information
    float year = scene->getSimYear();
    float CO2 = scene->rcp8_5.getConcentration(year);
    ImGui::Text("Year: %.0f (CO2: %.0fppm)\n", year, CO2);
    
    if(scene->simulationActive())
      ImGui::Text("Simulation Speed: %.2f\n", scene->simulationSpeed);
    else
      ImGui::Text("Simulation: Paused\n");
    
    // Season selection
    if(ImGui::Button("Spring"))
     {
      LogSimulationControls("Spring season selected.\n");
      displaySeason = 0u;
     }
    ImGui::SameLine();
    if(ImGui::Button("Summer"))
     {
      LogSimulationControls("Summer season selected.\n");
      displaySeason = 1u;
     }
    ImGui::SameLine();
    if(ImGui::Button("Fall"))
     {
      LogSimulationControls("Fall season selected.\n");
      displaySeason = 2u;
     }
    ImGui::SameLine();
    if(ImGui::Button("Winter"))
     {
      LogSimulationControls("Winter season selected.\n");
      displaySeason = 3u;
     }
    
    ImGui::Separator();
   }
  ImGui::End();
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
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  imguiInsertMenu();
  imguiTreeMenu();
  imguiAllTreeSelector();
  imguiHeightInputDialog();
  if(shedPanel)
    shedPanel->display();
  if(blockPanel)
    blockPanel->display();
  
  imguiSimulationController();
  imguiFocusOverlay();
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
  if(show_tree_menu)
   {
    for(auto& iter: Species::genusList)
     {
      if(strcmp(iter.first.c_str(), "Nosuchgenus") == 0)
        continue;
      httPrintf("%s (%u)\n", iter.first.c_str(), iter.second);
     }
    httPrintf("All Tree Selector\n");
   }
  else if(all_tree_selector && currentList)
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
   }
  else
   {
    httPrintf("OK\n");
    //err(-1, "MenuInterface::HTTPAPiOptions called on unimplemented menu.");
   }
  return true;
}


// =======================================================================================
// Function to handle action creation/error reporting.

bool MenuInterface::createAction(HttpDebug* serv, ActionType actionType, 
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
// Helper function to restore spaces in the path

void unencode(char* path)
{
  for(char* p = path; *p; p++)
    if(*p == '_')
      *p = ' ';
}


// =======================================================================================
// Function to handle selecting a genus from the insert tree selection menu.

bool MenuInterface::HTTPAPiSelectGenus(HttpDebug* serv, char* path)
{
  unencode(path);
  
  if(!show_tree_menu)
   {
    LogRequestErrors("MenuInterface::HTTPAPiSelectGenus with show_tree_menu false.\n");
    return false;    
   }
  
  if(strncmp(path, "All Tree Selector", 17) == 0 || Species::genusList.count(path) )
    return createAction(serv, SelectGenus, (char*)"SelectGenus", 
                                                        (char*)"HTTPAPiSelectGenus", path);
  
  LogRequestErrors("MenuInterface::HTTPAPiSelectGenus unknown selection %s\n", path);
  return false;
}


// =======================================================================================
// Function to handle selecting a tree via the all_tree_selector tree menus

bool MenuInterface::HTTPAPiAllTreeSelector(HttpDebug* serv, char* path)
{
  unencode(path);
  
  if(!all_tree_selector)
   {
    LogRequestErrors("MenuInterface::HTTPAPiAllTreeSelector with "
                                                            "all_tree_selector false.\n");
    return false;    
   }
  
  if(strlen(path) > 128)
   {
    // Weird long paths get cut off here.  Otherwise path checking has to be done
    // downstream from us.
    LogRequestErrors("MenuInterface::HTTPAPiAllTreeSelector doesn't like long path.\n");
    return false;        
   }
  
  return createAction(serv, AllTreeSelection, (char*)"AllTreeSelection", 
                                                        (char*)"HTTPAPiAllTreeSelector", path);
}


// =======================================================================================
// Function to handle parsing simulation panel interface actions.

bool MenuInterface::HTTPAPiSimulate(HttpDebug* serv, char* path)
{
  if(strncmp(path, "start", 5)== 0)
    return createAction(serv, SimulateStart, (char*)"SimulateStart", 
                                                        (char*)"HTTPAPiSimulate", path+5);

  if(strncmp(path, "pause", 5)== 0)
    return createAction(serv, SimulatePause, (char*)"SimulatePause", 
                                                        (char*)"HTTPAPiSimulate", path+5);

  else if(strncmp(path, "reset", 5)== 0)
    return createAction(serv, SimulateReset, (char*)"SimulateReset", 
                                                        (char*)"HTTPAPiSimulate", path+5);

  else if(strncmp(path, "spring", 6)== 0)
    return createAction(serv, SimulateSpring, (char*)"SimulateSpring", 
                                                        (char*)"HTTPAPiSimulate", path+6);

  else if(strncmp(path, "summer", 6)== 0)
    return createAction(serv, SimulateSummer, (char*)"SimulateSummer", 
                                                        (char*)"HTTPAPiSimulate", path+6);

  else if(strncmp(path, "fall", 4)== 0)
    return createAction(serv, SimulateFall, (char*)"SimulateFall", 
                                                        (char*)"HTTPAPiSimulate", path+4);

  else if(strncmp(path, "winter", 6)== 0)
    return createAction(serv, SimulateWinter, (char*)"SimulateWinter", 
                                                        (char*)"HTTPAPiSimulate", path+6);

  
  LogRequestErrors("MenuInterface::HTTPAPi unknown simulation command %s\n", path);
  return false;
}


// =======================================================================================
// Function to handle parsing simulation panel interface actions.

bool MenuInterface::HTTPAPiInsert(HttpDebug* serv, char* path)
{
  unless(show_insert_menu)
   {
    LogRequestErrors("MenuInterface::HTTPAPiInsert called when no insert menu showing.\n");
    return false;
   }
  
  if(strncmp(path, "height", 6) == 0)
    return createAction(serv, InsertHeight, (char*)"InsertHeight", 
                                                          (char*)"HTTPAPiInsert", path+6);

  if(strncmp(path, "block", 5) == 0)
    return createAction(serv, InsertBlock, (char*)"InsertBlock", 
                                                            (char*)"HTTPAPiInsert", path+5);

  if(strncmp(path, "shed", 4) == 0)
    return createAction(serv, InsertShed, (char*)"InsertShed", 
                                                            (char*)"HTTPAPiInsert", path+4);

  if(strncmp(path, "gable", 5) == 0)
    return createAction(serv, InsertGable, (char*)"InsertGable", 
                                                            (char*)"HTTPAPiInsert", path+5);

  if(strncmp(path, "tree", 4) == 0)
    return createAction(serv, InsertTree, (char*)"InsertTree", 
                                                          (char*)"HTTPAPiInsert", path+4);

  LogRequestErrors("MenuInterface::HTTPAPiInsert unknown insert command %s\n", path);
  return false;
}


// =======================================================================================
// Function to handle entered parameters.

bool MenuInterface::HTTPAPiEnter(HttpDebug* serv, char* path)
{  
  if(show_height_input_dialog && strncmp(path, "height/", 7) == 0)
    return createAction(serv, HeightEntered, (char*)"HeightEntered", 
                                                          (char*)"HTTPAPiEnter", path+7);

  if(blockPanel && strncmp(path, "block/", 6) == 0)
    return createAction(serv, BlockEntered, (char*)"BlockEntered", 
                                                          (char*)"HTTPAPiEnter", path+6);

  LogRequestErrors("MenuInterface::HTTPAPiEnter incorrect enter command %s\n", path);
  return false;
}


// =======================================================================================
// Function to handle the parsing of selection actions (ie everything under /select comes
// here first.

bool MenuInterface::HTTPAPiSelections(HttpDebug* serv, char* path)
{  
  if(strncmp(path, "genus/", 6)== 0)
     return HTTPAPiSelectGenus(serv, path+6);

  if(strncmp(path, "alltree/", 8)== 0)
     return HTTPAPiAllTreeSelector(serv, path+8);
  
  LogRequestErrors("MenuInterface::HTTPAPiSelections unknown directive %s\n", path);
  return false;
}


// =======================================================================================
// Function to handle the parsing of interface actions coming from HTTP (ie for test
// scripts).

bool MenuInterface::HTTPAPi(HttpDebug* serv, char* path)
{  
  if(strncmp(path, "simulate/", 9)== 0)
     return HTTPAPiSimulate(serv, path+9);

  if(strncmp(path, "insert/", 7)== 0)
    return HTTPAPiInsert(serv, path+7);

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
