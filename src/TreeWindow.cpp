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
/// @param existing A pointer to an existing window, with which this new window should
/// share an OpenGL context.

TreeWindow::TreeWindow(Tree& tree, vec3& camFront, Window3D* existing):
                      ObjectInspectorWindow(tree, camFront, TREE_WINDOW_INIT_WIDTH, 
                      TREE_WINDOW_INIT_HEIGHT, (char*)"Tree Inspector", existing)
{
}


// =======================================================================================
/// @brief Destructor

TreeWindow::~TreeWindow(void)
{
}


// =======================================================================================
/// @brief Function called in the HTTP server thread to provide one row of a table about
/// our particular window instance.  
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool TreeWindow::diagnosticHTMLRow(HttpDebug* serv, int rowIndex)
{
  httPrintf("<tr><td>%d</td>"
            "<td><a href=\"/window/detail/%d/\">%s</a></td>"
            "<td>Details</td></tr>\n", rowIndex, ourWin, winTitle);
  return true;
}


// =======================================================================================
/// @brief Render the scene into our window.
/// @param timeInterval Number of seconds since last iteration in the window loop.

void TreeWindow::draw(float timeInterval)
{
  scene->draw(camera, timeInterval);
  //imgMenu->imguiInterface();
}


// =======================================================================================
