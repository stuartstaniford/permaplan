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
#include "MenuInterface.h"
#include "ScriptController.h"
#include "Timeval.h"

class Window3D
{
  friend MenuInterface;
  
 public:
  
  // Public instance variables
  Scene*            scene;
  ScriptController* scriptController;
  MenuInterface*    imgMenu;

  // Public methods
  Window3D(int pixWidth, int pixHeight);
  void loop(HttpDebug& httpServer);
  bool diagnosticHTML(HttpDebug* serv);
  
 private:
  
  // Class variables - private
  
  // Instance variables - private
  int             width;
  int             height;
  double          lastMouseX, lastMouseY;
  GLFWwindow*     window;
  Timeval         lastTime;
  Timeval         mouseUpTime;
  Timeval         clickTime;
  float           clickLength;
  bool            inClick;
  bool            testingDoubleClick;
  bool            mouseMoved;
  float           frameTimeAvg;
  
  // Private methods
  void          processInput(Camera& camera);
  void          processMouse(Camera& camera);
  void          processClick(float mouseX, float mouseY);
  void          processDoubleClick(float mouseX, float mouseY, float timeDiff);
  void          processPseudoAction(InterfaceAction* action);
  float         timeDelta(void);           // number of microseconds since last call

  Window3D(const Window3D&);                 // Prevent copy-construction
  Window3D& operator=(const Window3D&);      // Prevent assignment

};

#endif
