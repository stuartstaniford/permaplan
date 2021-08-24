// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -
// Inspector window for a Tree.
// A window to look in detail at a particular tree, abstracted from the landscape in 
// which it is embedded.

#include "TreeWindow.h"


// =======================================================================================
/// @brief Constructor

TreeWindow::TreeWindow(void):
                        ObjectInspectorWindow(TREE_WINDOW_INIT_WIDTH, 
                                        TREE_WINDOW_INIT_HEIGHT, (char*)"Tree Inspector")
{
}


// =======================================================================================
/// @brief Destructor

TreeWindow::~TreeWindow(void)
{
}


// =======================================================================================
