// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class handles the UI panel for inserting a block.

#include "MenuBlockPanel.h"
#include "Material.h"
#include "MenuInterface.h"


// =======================================================================================
/// @brief Constructor
///
/// The constructor is mostly a pass through to MenuPanel, but also is where the 
/// displayVisible is set.
/// @param menu Pointer to the overall MenuInterface so we can call back into it. 
/// @param S A pointer to the scene.

MenuBlockPanel::MenuBlockPanel(MenuInterface* menu, Scene* S):
                                      MenuPanel(menu, S)
{
  sizeBuf[0] = '\0';
  displayVisible = true;
}


// =======================================================================================
// Destructor

MenuBlockPanel::~MenuBlockPanel(void)
{
}


// =======================================================================================
/// @brief - provide the imGui interface for the BlockPanel.

void MenuBlockPanel::imGuiDisplay(void)
{
  if(!displayVisible)
    return;
  ImGui::Begin("Block Size and Material", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);

  ImGui::InputText("", sizeBuf, 8, ImGuiInputTextFlags_CharsDecimal);

  const MaterialList& materials = MaterialList::getMaterials();
  
  for(auto& iter: materials)
    if(ImGui::Button(iter.first.c_str()))
     {
      size = atof(sizeBuf);
      sizeBuf[0] = '\0';
      mainMenu->blockEnteredButton(size, iter.first);
      delete this;
      ImGui::End();
      return;
     }
  ImGui::End();
}


// =======================================================================================
