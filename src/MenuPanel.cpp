// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is an abstract superclass for menus and panels in the menu interface
// system.  Individual menus should inherit from this and conform to this interface

#include "MenuPanel.h"
#include <err.h>

// =======================================================================================
/// @brief Constructor for the MenuPanel that initializes the generic variables
/// 
/// @param menu Pointer to the overall MenuInterface so we (and our subclasses) can 
/// call back into it. 

MenuPanel::MenuPanel(MenuInterface* menu):
                  displayVisible(false),
                  mainMenu(menu),
                  errPanel(NULL)
{
}


// =======================================================================================
/// @brief Destructor
///

MenuPanel::~MenuPanel(void)
{
}


// =======================================================================================
/// @brief Interface for subclass function to check that all panel fields are complete
/// and correct.  
/// 
/// The version in this clsss will always return an error.  Subclasses should override
/// this.
///
/// @return A pointer to a char array with an error message.  It should be NULL if there
/// is no error.

char* MenuPanel::errorInFields(void)
{
  static char* errString = (char*)"Unimplemented superclass method "
                                                              "MenuPanel::errorInFields.\n";
  return errString;
}


// =======================================================================================
/// @brief Interface for displaying the panel when it is supposed to be on screen.  
/// 
/// The version in this clsss will always return an error.  Subclasses should override
/// this.

void MenuPanel::display(void)
{
  err(-1, "Unimplemented superclass MenuPanel::diaplsy");
}


// =======================================================================================
/// @brief Method for setting the corner of menus in the corners of the window

void MenuPanel::setCorner(int& corner)
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
/// @brief Method for setting menu in the center of the window

void MenuPanel::setCenter(void)
{
  ImGuiIO& io = ImGui::GetIO();
  ImVec2 window_pos = ImVec2(io.DisplaySize.x/2, io.DisplaySize.y/2);
  ImVec2 window_pos_pivot = ImVec2(0.5f, 0.5f);
  ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
}


// =======================================================================================
