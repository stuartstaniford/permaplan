// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -
// Functionality for all windows that serve to allow focussed inspection/manipulation 
// of a particular object in the scene. For example, these might be triggered by 
// double-clicking on the object in the main scene window.  It is not expected that 
// bare instances of this class will be created, but rather that the class will be 
// subclassed with additional functionality associated with that particular kind 
// of object.  But this class has functionality that is the same across any object to 
// be manipulated/displayed in an inspector window.

#include "ObjectInspectorWindow.h"
#include "VisualObject.h"


// =======================================================================================
/// @brief Constructor
///
/// @param obj The principle object that this window is being invoked to 
/// inspect/manipulate.
/// @param camFront A reference to the front vector of the camera in the calling 
/// scene window.  This will be used to ensure that we open up looking at the object 
/// from the same angle in the horizontal plane.
/// @param pixWidth The number of pixels wide to create this window.
/// @param pixHeight The number of pixels high to create this window.
/// @param title A C string for what to put on the window title bar.
/// @param existing A pointer to an existing window, with which this new window should
/// share an OpenGL context.  This is NULL by default (creating a new context).

ObjectInspectorWindow::ObjectInspectorWindow(VisualObject& obj, vec3& camFront, int pixWidth, 
                    int pixHeight, const char* title, GLFWApplication& app, Window3D* existing):
                                            Window3D(pixWidth, pixHeight, title, app, existing),
                                            object(obj)
{
  camera.focusOnObject(obj.box, camFront, pixWidth, pixHeight);
}


// =======================================================================================
/// @brief Destructor

ObjectInspectorWindow::~ObjectInspectorWindow(void)
{
}


// =======================================================================================
