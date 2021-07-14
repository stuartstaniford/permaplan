// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class ..... is a template for real classes

#include "MenuAllTree.h"


// =======================================================================================
/// @brief Constructor
///
/// The constructor is mostly a pass through to MenuPanel.
/// @param menu Pointer to the overall MenuInterface so we can call back into it. 
/// @param S A pointer to the scene.

MenuAllTree::MenuAllTree(MenuInterface* menu, Scene* S):
                                      MenuPanel(menu, S)
{
}


// =======================================================================================
/// @brief Destructor

MenuAllTree::~MenuAllTree(void)
{
}


// =======================================================================================
