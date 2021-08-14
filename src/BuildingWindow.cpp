// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -
// Inspector window to allow manipulation of an individual building
// This subclass of Window3D is specialized to allow interaction with a single object
// of type Building for the purpose of messing with the building, adding gables, sheds,
// etc.

#include "BuildingWindow.h"


// =======================================================================================
/// @brief Constructor

BuildingWindow::BuildingWindow(void):
                Window3D(BUILDING_WINDOW_INIT_WIDTH, BUILDING_WINDOW_INIT_HEIGHT,
                         (char*)"Building Editor")
{
}


// =======================================================================================
/// @brief Destructor

BuildingWindow::~BuildingWindow(void)
{
}


// =======================================================================================
