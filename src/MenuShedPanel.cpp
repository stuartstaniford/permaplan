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
// The floating menu to set up parameters for an inserted shed

void MenuShedPanel::display(void)
{
  if(!displayVisible)
    return;
  ImGui::Begin("Insert a Shed", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::Text("Length (%c):", spaceUnitAbbr);
  ImGui::SameLine();
  ImGui::InputText("", lengthBuf, 8, ImGuiInputTextFlags_CharsDecimal);

  ImGui::Text("Width (%c):", spaceUnitAbbr);
  ImGui::SameLine();
  ImGui::InputText("", widthBuf, 8, ImGuiInputTextFlags_CharsDecimal);

  ImGui::Text("Height (%c):", spaceUnitAbbr);
  ImGui::SameLine();
  ImGui::InputText("", heightBuf, 8, ImGuiInputTextFlags_CharsDecimal);

  
  ImGui::End();
}




// =======================================================================================
