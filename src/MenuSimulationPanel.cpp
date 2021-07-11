// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class provides for the main panel that is used to stop, start, and pause the
// simulation, and also displays basic status of it (like the year and the CO2
// concentration. It's a transparent overlay in the top right corner.

#include "MenuSimulationPanel.h"
#include "Global.h"
#include "PmodDesign.h"
#include "MenuInterface.h"
#include "MenuErrorPanel.h"
#include "LeafModel.h"


// =======================================================================================
// Constants, static variable initializations, etc


// =======================================================================================
/// @brief Constructor
///
/// The constructor is where a variety of default values for the inserted shed are set. 
/// It also initializes a number of static variables if they weren't done before.
///
/// @param menu Pointer to the overall MenuInterface so we can call back into it. 
/// @param S A reference to the scene->

MenuSimulationPanel::MenuSimulationPanel(MenuInterface* menu, Scene* S):
                                            MenuPanel(menu, S)
{
  displayVisible = true; // simulation panel shows by default
}


// =======================================================================================
/// @brief Destructor
///
/// Presently empty/default.

MenuSimulationPanel::~MenuSimulationPanel(void)
{
}


// =======================================================================================
/// @brief Verifies the validity of the panel state.
/// 
/// This function verifies that all state variables of the MenuSimulationdPanel are 
/// consistent and complete.  If not, it provides a diagnostic error string.  Note in 
/// some cases it relies on a static buffer and thus is not thread-safe.
///
/// @return A char* pointer to a diagnostic error string.

char* MenuSimulationPanel::errorInFields(void)
{            
  return NULL;
}


// =======================================================================================
/// @brief Display the panel.
///
/// Calls all the various ImGui calls to actually put the panel together.

void MenuSimulationPanel::display(void)
{
  if(!displayVisible)
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
  if (ImGui::Begin("Simulation Control", &displayVisible, window_flags))
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
      simulateStart();
    ImGui::SameLine();
    if(ImGui::Button("Pause"))
      simulatePause();
    ImGui::SameLine();
    if(ImGui::Button("Reset"))
      simulateReset();
    
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
      simulateSpring();
    ImGui::SameLine();
    if(ImGui::Button("Summer"))
      simulateSummer();
    ImGui::SameLine();
    if(ImGui::Button("Fall"))
      simulateFall();
    ImGui::SameLine();
    if(ImGui::Button("Winter"))
      simulateWinter();
    
    ImGui::Separator();
   }
  ImGui::End();
}


// =======================================================================================
/// @brief Function to handle parsing simulation panel interface requests from the HTTP 
/// diagnostic server.  
/// 
/// Note that this will *not* be called on the main thread, but rather one
/// of the HTTP server threads.  
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.
/// @param path The last part of the URL that is specific to this particular panel (the path
/// required to navigate to us is hidden from us because it's not our concern.

bool MenuSimulationPanel::handleHTTPRequest(HttpDebug* serv, char* path)
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
/// @brief Interface for the method to be called in the main thread to process an 
/// simulation panel related interface action.
/// 
/// Note the action will have originally arisen in the HTTP server and have been 
/// processed by MenuSimulationPanel::handleHTTPRequest).  
/// @returns The ActionType of the action we processed.
/// @param action The InterfaceAction that needs to be handled.

ActionType MenuSimulationPanel::processAction(InterfaceAction* action)
{
  switch(action->actionType)
   {
    case SimulatePause:
      LogPseudoActions("Pause button press processed.\n");
      simulatePause();
      return SimulatePause;
   
    case SimulateReset:
      LogPseudoActions("Reset button press processed.\n");
      simulateReset();
      return SimulateReset;

    case SimulateStart:
      LogPseudoActions("Simulate start button press processed.\n");
      simulateStart();
      return SimulateStart;

    case SimulateSpring:
      LogPseudoActions("Simulate spring button press processed.\n");
      simulateSpring();
      return SimulateSpring;

    case SimulateSummer:
      LogPseudoActions("Simulate summer button press processed.\n");
      simulateSummer();
      return SimulateSummer;

    case SimulateFall:
      LogPseudoActions("Simulate fall button press processed.\n");
      simulateFall();
      return SimulateFall;

    case SimulateWinter:
      LogPseudoActions("Simulate winter button press processed.\n");
      simulateWinter();
      return SimulateWinter;

    default:
      return NoAction;
   }
}


// =======================================================================================
/// @brief Pause the simulation.
///
/// This is the abstract interface operation to pause the overall permaplan simulation.
/// All interfaces should route through here in the end to perform this action.

void MenuSimulationPanel::simulatePause(void)
{
  LogSimulationControls("Pause Button \xe2\x8f\xb8 pressed.\n");
  scene->pauseSimulation();
}


// =======================================================================================
/// @brief Reset the simulation.
///
/// This is the abstract interface operation to reset the overall permaplan simulation
/// (that is, lose the current state and go back to the state at the beginning).  All 
/// interfaces should route through here in the end to perform this action.
/// @todo Although this function is implemented, the underlying functionality in Scene
/// is not complete yet, so this doesn't work.

void MenuSimulationPanel::simulateReset(void)
{
  LogSimulationControls("Reset Button \xe2\x8f\xae pressed.\n");
  scene->resetSimulation();
}


// =======================================================================================
/// @brief Start the simulation.
///
/// This is the abstract interface operation to start the overall permaplan simulation.
/// When pressed for the first time (or after a reset), it starts from the beginning.
/// After a pause operation, it restarts from the state at the time of pause (possibly
/// as modified by the user in the interim).  All interfaces should route through here 
/// in the end to perform this action.

void MenuSimulationPanel::simulateStart(void)
{
  LogSimulationControls("Simulate Button \xe2\x96\xb6 pressed.\n");
  scene->startSimulation();
}


// =======================================================================================
/// @brief Set the simulation display to spring.
///
/// This is the abstract interface operation to set the simulation display to spring 
/// (this has an effect on foliage color and the like).  All interfaces should route 
/// through here in the end to perform this action.

void MenuSimulationPanel::simulateSpring(void)
{
  LogSimulationControls("Spring season selected.\n");
  displaySeason = 0u;
}


// =======================================================================================
/// @brief Set the simulation display to summer.
///
/// This is the abstract interface operation to set the simulation display to summer 
/// (this has an effect on foliage color and the like).  All interfaces should route 
/// through here in the end to perform this action.

void MenuSimulationPanel::simulateSummer(void)
{
  LogSimulationControls("Summer season selected.\n");
  displaySeason = 1u;
}


// =======================================================================================
/// @brief Set the simulation display to fall (aka autumn).
///
/// This is the abstract interface operation to set the simulation display to fall 
/// (this has an effect on foliage color and the like).  All interfaces should route 
/// through here in the end to perform this action.

void MenuSimulationPanel::simulateFall(void)
{
  LogSimulationControls("Fall season selected.\n");
  displaySeason = 2u;
}


// =======================================================================================
/// @brief Set the simulation display to winter.
///
/// This is the abstract interface operation to set the simulation display to winter 
/// (this has an effect on foliage color and the like).  All interfaces should route 
/// through here in the end to perform this action.

void MenuSimulationPanel::simulateWinter(void)
{
  LogSimulationControls("Winter season selected.\n");
  displaySeason = 3u;
}


// =======================================================================================
