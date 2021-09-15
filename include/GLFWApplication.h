// Copyright Staniford Systems.  All Rights Reserved.  September 2021 -

#ifndef GLFW_APPLICATION_H
#define GLFW_APPLICATION_H

#include "Lockable.h"


// =======================================================================================
// Forward declarations

class Window3D;


// =======================================================================================
/// @brief Overall organization of an application consisting of multiple GLFW Windows.
/// 
/// This manages the overall rendering and simulation loop and keeps track of which 
/// windows are currently open/on-screen.  It's very specific to OpenGL/GLFW.

class GLFWApplication: public Lockable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  GLFWApplication(void);
  ~GLFWApplication(void);
  void addWindow(Window3D& window);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  GLFWApplication(const GLFWApplication&);       
  /// @brief Prevent assignment.
  GLFWApplication& operator=(const GLFWApplication&);      

};

#endif




