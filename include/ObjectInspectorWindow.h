// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -

#ifndef OBJECT_INSPECTOR_WINDOW_H
#define OBJECT_INSPECTOR_WINDOW_H

#include "Window3D.h"


// =======================================================================================
/// @brief Functionality for all windows that serve to allow focussed 
/// inspection/manipulation of a particular object in the scene.
/// 
/// For example, these might be triggered by double-clicking on the object in the main
/// scene window.  It is not expected that bare instances of this class will be created,
/// but rather that the class will be subclassed with additional functionality associated
/// with that particular kind of object.  But this class has functionality that is the 
/// same across any object to be manipulated/displayed in an inspector window.


class ObjectInspectorWindow: public Window3D
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ObjectInspectorWindow(VisualObject& obj, vec3& camFront, int pixWidth, int pixHeight, 
                                                                        const char* title);
  ~ObjectInspectorWindow(void);

protected:
  
  // Instance variables - protected
  VisualObject& object;

private:
    
  // Member functions - private
  /// @brief Prevent copy-construction.
  ObjectInspectorWindow(const ObjectInspectorWindow&);       
  /// @brief Prevent assignment.
  ObjectInspectorWindow& operator=(const ObjectInspectorWindow&);      

};

#endif




