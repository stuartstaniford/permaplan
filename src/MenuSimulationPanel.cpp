// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class handles the UI panel for inserting a shed.

#include "MenuSimulationPanel.h"
#include "Global.h"
#include "PmodDesign.h"
#include "MenuInterface.h"
#include "MenuErrorPanel.h"


// =======================================================================================
// Constants, static variable initializations, etc


// =======================================================================================
/// @brief Constructor
///
/// The constructor is where a variety of default values for the inserted shed are set. 
/// It also initializes a number of static variables if they weren't done before.
///
/// @param menu Pointer to the overall MenuInterface so we can call back into it. 


MenuSimulationPanel::MenuSimulationPanel(MenuInterface* menu):
                                            MenuPanel(menu)
{
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
}


// =======================================================================================
