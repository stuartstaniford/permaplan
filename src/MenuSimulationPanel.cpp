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


MenuSimulationPanel::MenuSimulationPanel(MenuInterface* menu, Scene& S):
                                            MenuPanel(menu),
                                            scene(S)
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
     {
      LogSimulationControls("Simulate Button \xe2\x96\xb6 pressed.\n");
      scene.startSimulation();
     }
    ImGui::SameLine();
    if(ImGui::Button("Pause"))
     {
      LogSimulationControls("Pause Button \xe2\x8f\xb8 pressed.\n");
      scene.pauseSimulation();
     }
    ImGui::SameLine();
    if(ImGui::Button("Reset"))
     {
      LogSimulationControls("Reset Button \xe2\x8f\xae pressed.\n");
      scene.resetSimulation();
     }
    
    // Auxiliary status information
    float year = scene.getSimYear();
    float CO2 = scene.rcp8_5.getConcentration(year);
    ImGui::Text("Year: %.0f (CO2: %.0fppm)\n", year, CO2);
    
    if(scene.simulationActive())
      ImGui::Text("Simulation Speed: %.2f\n", scene.simulationSpeed);
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
