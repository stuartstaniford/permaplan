// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class handles the UI panel for inserting a block.

#include "MenuBlockPanel.h"
#include "Material.h"
#include "MenuInterface.h"

// =======================================================================================
// Constructor

MenuBlockPanel::MenuBlockPanel(MenuInterface* menu, Scene* S):
                                      MenuPanel(menu, S)
{
  sizeBuf[0] = '\0';
}


// =======================================================================================
// Destructor

MenuBlockPanel::~MenuBlockPanel(void)
{
}


// =======================================================================================
// The floating menu to select a material (eg for a block)

void MenuBlockPanel::display(void)
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
      return;
     }
  ImGui::End();
}


// =======================================================================================
