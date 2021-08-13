// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -
// This subclass of Window3D is specialized to view the scene as a whole, and is the 
// first window that comes up.  Other more specialized inspector windows can be spawned
// from this.

#include "MainSceneWindow.h"


// =======================================================================================
/// @brief Constructor

MainSceneWindow::MainSceneWindow(int pixWidth, int pixHeight):
                            Window3D(pixWidth, pixHeight)
{
}


// =======================================================================================
/// @brief Destructor

MainSceneWindow::~MainSceneWindow(void)
{
}


// =======================================================================================
