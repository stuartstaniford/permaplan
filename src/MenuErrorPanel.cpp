// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class if for an error warning that needs to be thrown up and block the interface
// (eg because there are bad parameters in another panel, and the user needs to confirm
// their intent to fix them before continuing).

#include "MenuErrorPanel.h"
#include <cstring>

// =======================================================================================
// Constructor

MenuErrorPanel::MenuErrorPanel(const char* errIn):
                                    displayVisible(false)
{
  strncpy(errString, errIn, ERR_PANEL_MAX);
}


// =======================================================================================
// Destructor

MenuErrorPanel::~MenuErrorPanel(void)
{
}


// =======================================================================================
// Function to display the error, and a button to acknowledge it.

bool MenuErrorPanel::displayDone(void)
{
  if(!displayVisible)
    return false;
  ImGui::Begin("Input Error", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);

  // Display the error
  ImGui::Text("%s", errString);

  // Bottom row of buttons to cancel/enter.
  if(ImGui::Button("Got it."))
   {
    delete this;
    return true;
   }
  
  ImGui::End();
  return false;
}


// =======================================================================================
