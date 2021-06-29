// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class handles the UI panel for inserting a shed.

#include "MenuShedPanel.h"
#include "Global.h"
#include "PmodDesign.h"

// =======================================================================================
// Constructor

MenuShedPanel::MenuShedPanel(MenuInterface* menu):
                              MenuPanel(menu)
{
  heightBuf[0] = '\0'; 
  lengthBuf[0] = '\0'; 
  widthBuf[0] = '\0';
}


// =======================================================================================
// Destructor

MenuShedPanel::~MenuShedPanel(void)
{
}


// =======================================================================================
// Function to confirm 

char* MenuShedPanel::errorInFields(void)
{
  if(strlen(heightBuf)<1)
    return (char*)"No height data entered.";
  if(strlen(lengthBuf)<1)
    return (char*)"No length data entered.";
  if(strlen(widthBuf)<1)
    return (char*)"No width data entered.";
  
  static char errBuf[48];
  char* errString;
  
  if((errString = errorInNumericCharacterField(heightBuf)))
   {
    sprintf(errBuf, "Bad height value: %s", errString);
    return errBuf;
   }

  if((errString = errorInNumericCharacterField(lengthBuf)))
   {
    sprintf(errBuf, "Bad length value: %s", errString);
    return errBuf;
   }

  if((errString = errorInNumericCharacterField(lengthBuf)))
   {
    sprintf(errBuf, "Bad length value: %s", errString);
    return errBuf;
   }
            
  return NULL;
}


// =======================================================================================
// The floating menu to set up parameters for an inserted shed

void MenuShedPanel::display(void)
{
  if(!displayVisible)
    return;
  ImGui::Begin("Insert a Shed", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);

  // Text fields for data entry
  ImGui::Text("Length (%c):", spaceUnitAbbr);
  ImGui::SameLine();
  ImGui::InputText("", lengthBuf, 8, ImGuiInputTextFlags_CharsDecimal);

  ImGui::Text("Width (%c):", spaceUnitAbbr);
  ImGui::SameLine();
  ImGui::InputText("", widthBuf, 8, ImGuiInputTextFlags_CharsDecimal);

  ImGui::Text("Height (%c):", spaceUnitAbbr);
  ImGui::SameLine();
  ImGui::InputText("", heightBuf, 8, ImGuiInputTextFlags_CharsDecimal);

  // Bottom row of buttons to cancel/enter.
  if(ImGui::Button("Cancel"))
   {

   }
  ImGui::SameLine();
  if(ImGui::Button("Create Shed"))
   {
    char* errString;
    if((errString = errorInFields()))
     {
      
     }
   }

  
  ImGui::End();
}




// =======================================================================================
