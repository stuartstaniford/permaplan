// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Utility class for opening a window and setting up for drawing in using
// OpenGL commands.  The present implementation uses GLFW but the API should
// hide that.

#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "MenuInterface.h"
#include "Material.h"
#include "Species.h"
#include "Window3D.h"
#include "loadFileToBuf.h"

// =======================================================================================
// Constructor for a Window3D

MenuInterface::MenuInterface(GLFWwindow* window, Window3D& W):
                        scene(NULL),
                        show_insert_menu(false),
                        show_lock_overlay(false),
                        win3D(W),
                        show_height_input_dialog(false),
                        show_materials_menu(false),
                        show_tree_menu(false),
                        genusSelected(NULL),
                        show_focus_overlay(true),
                        show_simulation_controller(true),
                        all_tree_selector(false),
                        globalRegionsLoaded(false)
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

  heightBuf[0] = '\0'; // clear the buffer
}


// =======================================================================================
// Utility function for handling menus in the corners of the window

void setCorner(int& corner)
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
  show_materials_menu = true;
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
  if(ImGui::Button("Tree"))
    insertTreeButton();

  ImGui::End();
}


// =======================================================================================
// Block entered button action (also from HTTP debug interface)

void MenuInterface::blockEnteredButton(float blockSize, const std::string& matName)
{
  show_materials_menu = false;
  const MaterialList& materials = MaterialList::getMaterials();
  auto iter = materials.find(matName);
  scene->insertVisibleObject((char*)"Block", blockSize, scene->lastDoubleClick, iter->second);
  LogMaterialSelections("Material %s selected for block, carbon density %.2f.\n",
                        matName, iter->second->carbonDensity);
}


// =======================================================================================
// The floating menu to select a material (eg for a block)

void MenuInterface::imguiMaterialsMenu(void)
{
  if(!show_materials_menu)
    return;
  ImGui::Begin("Block Size and Material", &show_materials_menu, ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::InputText("", heightBuf, 8, ImGuiInputTextFlags_CharsDecimal);

  const MaterialList& materials = MaterialList::getMaterials();
  
  for(auto& iter: materials)
    if(ImGui::Button(iter.first.c_str()))
     {
      size = atof(heightBuf);
      heightBuf[0] = '\0';
      blockEnteredButton(size, iter.first);
     }
  ImGui::End();
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
      genusSelected = iter.first.c_str();
   }
  if(ImGui::Button("All Tree Selector"))
   {
    genusSelected = NULL;
    show_tree_menu = false;
    all_tree_selector = true;
   }

  ImGui::End();
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

void MenuInterface::imguiAllTreeSelector(void)
{
  if(!all_tree_selector)
    return;

  unless(globalRegionsLoaded)
   {
    unsigned bufSize;
    char* fileName = (char*)"Materials/Trees/Regions/global.json";
    char* buf = loadFileToBuf(fileName, &bufSize);
    
    ParseResult ok = globalRegions.ParseInsitu<kParseCommentsFlag>(buf);
    if (!ok)
     {
      fprintf(stderr, "JSON parse error on %s: %s (%u)\n", fileName,
              GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
      exit(1);
     }
    if(!globalRegions.IsObject())
      err(-1, "Base of file %s is not JSON object.\n", fileName);

    globalRegionsLoaded = true;
   }
  ImGui::Begin("Tree Regions", &all_tree_selector, ImGuiWindowFlags_AlwaysAutoResize);
  
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
// Top level function to organize the rendering of all the imguiInterface stuff

void MenuInterface::imguiInterface(void)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  imguiInsertMenu();
  imguiMaterialsMenu();
  imguiTreeMenu();
  imguiAllTreeSelector();
  imguiHeightInputDialog();
  
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

  if(show_materials_menu && strncmp(path, "block/", 6) == 0)
    return createAction(serv, BlockEntered, (char*)"BlockEntered", 
                                                          (char*)"HTTPAPiEnter", path+6);

  LogRequestErrors("MenuInterface::HTTPAPiEnter incorrect enter command %s\n", path);
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
  
  LogRequestErrors("MenuInterface::HTTPAPi unknown directive %s\n", path);
  return false;
}


// =======================================================================================
