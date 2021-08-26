// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -
// Functionality for all windows that serve to allow focussed inspection/manipulation 
// of a particular object in the scene. For example, these might be triggered by 
// double-clicking on the object in the main scene window.  It is not expected that 
// bare instances of this class will be created, but rather that the class will be 
// subclassed with additional functionality associated with that particular kind 
// of object.  But this class has functionality that is the same across any object to 
// be manipulated/displayed in an inspector window.

#include "ObjectInspectorWindow.h"


// =======================================================================================
/// @brief Constructor

ObjectInspectorWindow::ObjectInspectorWindow(VisualObject& obj, int pixWidth, 
                                                          int pixHeight, const char* title):
                                            Window3D(pixWidth, pixHeight, title),
                                            object(obj)
{
}


// =======================================================================================
/// @brief Destructor

ObjectInspectorWindow::~ObjectInspectorWindow(void)
{
}


// =======================================================================================
