// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class provides for the display of a single UI panel that can be used when
// inserting or editing a gable building (or gable-like component of a larger building.) 

#include "MenuGablePanel.h"
#include "MenuInterface.h"
#include "MenuErrorPanel.h"


// =======================================================================================
// Constants, static variable initializations, etc

bool  MenuGablePanel::staticsInitialized = false; 
char  MenuGablePanel::heightLabel[16];
char  MenuGablePanel::lengthLabel[16];
char  MenuGablePanel::widthLabel[16];
char  MenuGablePanel::overhangLabel[16];


// =======================================================================================
/// @brief Constructor

MenuGablePanel::MenuGablePanel(MenuInterface* menu, Scene* S):
                                                      MenuPanel(menu, S),
                                                      height(18.0f),
                                                      length(40.0f),
                                                      width(30.0f),
                                                      roofAngle(15.0f),
                                                      overhang(2.0f)
{
  unless(staticsInitialized)
   {
    staticsInitialized = true;
    sprintf(heightLabel, "(%s)##h", spaceUnitName);
    sprintf(lengthLabel, "(%s)##l", spaceUnitName);
    sprintf(widthLabel, "(%s)##w", spaceUnitName);
    sprintf(overhangLabel, "(%s)##r", spaceUnitName);
   }
}


// =======================================================================================
/// @brief Destructor

MenuGablePanel::~MenuGablePanel(void)
{
}


// =======================================================================================
/// @brief Verifies the validity of the panel state.
/// 
/// This function verifies that all state variables of the MenuShedPanel are consistent
/// and complete.  If not, it provides a diagnostic error string.  Note in some cases it
/// relies on a static buffer and thus is not thread-safe.
///
/// @return A char* pointer to a diagnostic error string.


char* MenuGablePanel::errorInFields(void)
{
  static char errBuf[48];
  char* errString;
  
  if((errString = errorInFloat(height)))
   {
    sprintf(errBuf, "Bad height value: %s", errString);
    return errBuf;
   }

  if((errString = errorInFloat(length)))
   {
    sprintf(errBuf, "Bad length value: %s", errString);
    return errBuf;
   }

  if((errString = errorInFloat(width)))
   {
    sprintf(errBuf, "Bad width value: %s", errString);
    return errBuf;
   }
            
  return NULL;
}


// =======================================================================================
/// @brief Display the panel.
///
/// Calls all the various ImGui calls to actually put the panel together.

void MenuGablePanel::imGuiDisplay(void)
{
  if(!displayVisible)
    return;
  ImGui::Begin("Insert a Gable", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);

  // Text fields for data entry
  ImGui::Text("Length:");
  ImGui::SameLine();
  ImGui::InputFloat(lengthLabel, &length, 1.0f, 5.0f, "%.2f");

  ImGui::Text("Width:");
  ImGui::SameLine();
  ImGui::InputFloat(widthLabel, &width, 1.0f, 5.0f, "%.2f");

  ImGui::Text("Height");
  ImGui::SameLine();
  ImGui::InputFloat(heightLabel, &height, 1.0f, 5.0f, "%.2f");

  float maxAngle = atanf(height/width)*180.0f/M_PI;
  ImGui::Text("Roof Angle:");
  ImGui::SameLine();
  ImGui::SliderFloat("degrees", &roofAngle, 0.0f, maxAngle);

  ImGui::Text("Overhang");
  ImGui::SameLine();
  ImGui::InputFloat(overhangLabel, &overhang, 0.1f, 1.0f, "%.2f");

  // Bottom row of buttons to cancel/enter.
  if(ImGui::Button("Cancel"))
   {

   }
  ImGui::SameLine();
  if(ImGui::Button("Create Gable"))
   {
    char* errString;
    if((errString = errorInFields()))
     {
      errPanel = new MenuErrorPanel(errString);
     }
    else
     {
      ImGui::End();
      return;
     }
   }

  if(errPanel)
    if(errPanel->displayDone())
      errPanel = NULL;
  
  ImGui::End();
}


// =======================================================================================
// =======================================================================================
