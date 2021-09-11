// Copyright Staniford Systems.  All Rights Reserved.  September 2021 -
// This class is a special case of MenuPanel used only during startup when we need to 
// interact with the user about something (often something that has gone wrong).  It 
// is only called from Window3D::initPanel, which has it's own special rendering loop 
// because this is for use before the main rendering loop has started.

#include "MenuInitPanel.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


// =======================================================================================
/// @brief Constructor

MenuInitPanel::MenuInitPanel(void):
                            MenuPanel(NULL, NULL, NULL)
{
}


// =======================================================================================
/// @brief Destructor

MenuInitPanel::~MenuInitPanel(void)
{
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

int  MenuInitPanel::initPanel(char* question, char** responses, int nResponses)
{
  unless(displayVisible)
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
  if (ImGui::Begin("Dialog Box", &displayVisible, window_flags))
   {
    ImGui::Text("%s", question);
    ImGui::Separator();
    
    for(int i=0; i < nResponses; i++)
     {
      //fprintf(stderr, "Creating button %d\n",i);
      if(ImGui::Button(responses[i]))
       {
        displayVisible = false;
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
