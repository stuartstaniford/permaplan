// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class implements the floating menu to select a particular species to insert 
// after a genus has been selected in imguiTreeMenu.  Also gets the tree age at 
// planting time.

#include "MenuGenus.h"
#include "MenuInterface.h"
#include "Tree.h"

// =======================================================================================
/// @brief Constructor

MenuGenus::MenuGenus(MenuInterface* menu, Scene* S, const char* G):
                                        MenuPanel(menu, S),
                                        genusSelected(G)
{
  // clear the buffers
  ageBuf[0] = '\0'; 
}


// =======================================================================================
/// @brief Destructor

MenuGenus::~MenuGenus(void)
{
}


// =======================================================================================
/// @brief Operations performed when we are notified of a species selection
/// @todo  We are not doing any sanity checking on the tree age here.  Need to make 
/// sure it's numerical and non-negative.  Also need a default (eg 1.0f years)

void MenuGenus::speciesSelected(Species* S)
{
  LogTreeSelections("Tree %s %s (age %.1f years) inserted at [%f, %f].\n",
                  S->genusName, S->speciesName, atof(ageBuf),
                                    scene->lastDoubleClick[0], scene->lastDoubleClick[1]);
  Tree* tree = new Tree(S, scene->lastDoubleClick, atof(ageBuf), scene->getSimYear());
  scene->insertVisualObject(tree);
  mainMenu->genusMenu = NULL;
  delete this;
}


// =======================================================================================
/// @brief Display the genus selection menu via Dear ImGui.
///
/// Calls all the various ImGui calls to actually put the menu together.

void MenuGenus::imGuiDisplay(void)
{
  unless(displayVisible)
    return;

  ImGui::Begin("Tree Species", &displayVisible, ImGuiWindowFlags_AlwaysAutoResize);

  // Age field
  ImGui::Text("Tree age at planting: ");
  ImGui::SameLine();
  ImGui::InputText("1.0", ageBuf, 5, ImGuiInputTextFlags_CharsDecimal);

  //Species selector
  ImGui::Text("Species: ");
  for(auto& iter: *(Species::genusSpeciesList[genusSelected]))
   {
    if(strcmp(iter.first.c_str(), "nosuchspecies") == 0)
      continue;
    ImGui::SameLine();
    if(ImGui::Button(iter.first.c_str()))
      speciesSelected(iter.second);
   }
  ImGui::End();
}


// =======================================================================================
