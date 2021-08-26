// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -
// Inspector window for a Tree.
// A window to look in detail at a particular tree, abstracted from the landscape in 
// which it is embedded.

#include "TreeWindow.h"
#include "Tree.h"


// =======================================================================================
/// @brief Constructor
/// @param tree The Tree we are being called to inspect/manipulate.
/// @param camFront A reference to the front vector of the camera in the calling 
/// scene window.  This will be used to ensure that we open up looking at the tree 
/// from the same angle in the horizontal plane.

TreeWindow::TreeWindow(Tree& tree, vec3& camFront):
                      ObjectInspectorWindow(tree, camFront, TREE_WINDOW_INIT_WIDTH, 
                                        TREE_WINDOW_INIT_HEIGHT, (char*)"Tree Inspector")
{
}


// =======================================================================================
/// @brief Destructor

TreeWindow::~TreeWindow(void)
{
}


// =======================================================================================
