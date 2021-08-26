// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -
// Inspector window to allow manipulation of an individual building
// This subclass of Window3D is specialized to allow interaction with a single object
// of type Building for the purpose of messing with the building, adding gables, sheds,
// etc.

#include "BuildingWindow.h"
#include "Building.h"

// =======================================================================================
/// @brief Constructor
/// @param building The Building we are being called to inspect/manipulate.
/// @param camFront A reference to the front vector of the camera in the calling 
/// scene window.  This will be used to ensure that we open up looking at the building 
/// from the same angle in the horizontal plane.

BuildingWindow::BuildingWindow(Building& building, vec3& camFront):
                        ObjectInspectorWindow(building, camFront, BUILDING_WINDOW_INIT_WIDTH, 
                                    BUILDING_WINDOW_INIT_HEIGHT, (char*)"Building Editor")
{
}


// =======================================================================================
/// @brief Destructor

BuildingWindow::~BuildingWindow(void)
{
}


// =======================================================================================
