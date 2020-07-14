// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Utility class for opening a window and setting up for drawing in using
// OpenGL commands.  The present implementation uses GLFW but the API should
// hide that.

#ifndef WINDOW3D_H
#define WINDOW3D_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sys/time.h>
#include "Scene.h"
#include "Camera.h"
#include "imgui.h"

//#define SHOW_DEMO_WINDOW

class Window3D
{
 public:

  // Public instance variables
  Scene*  scene;

  // Public methods
  Window3D(int pixWidth, int pixHeight);
  void loop(void);
  void diagnosticHTML(void);
  
 private:
  
  // Class variables - private
  static bool  GLFWInitDone;
  
  // Instance variables - private
  int             width;
  int             height;
  double          lastMouseX, lastMouseY;
  GLFWwindow*     window;
  struct timeval  lastTime;
  bool            show_insert_menu;
  bool            show_focus_overlay;
#ifdef SHOW_DEMO_WINDOW
  bool            show_demo_window;
#endif
  char            heightBuf[16];
  
  // Private methods
  void    processInput(Camera& camera);
  void    processMouse(Camera& camera);
  float   timeDelta(void);              // returns the number of microseconds since last call
  void    imguiInterface(void);
  void    imguiInsertMenu(void);
  void    imguiFocusOverlay(void);

};

#endif
