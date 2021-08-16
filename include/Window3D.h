// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -

#ifndef WINDOW3D_H
#define WINDOW3D_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sys/time.h>
#include "Scene.h"
#include "Camera.h"
#include "MenuInterface.h"
#include "Timeval.h"
#include "HttpDebug.h"
#include <unordered_map>

// =======================================================================================
// Forward declarations

class MenuFocusOverlay;


// =======================================================================================
/// @brief
///
/// Utility class for opening a window and setting up for drawing in using OpenGL 
/// commands.  The present implementation uses GLFW but the API should hide that.

class Window3D
{
  friend MenuInterface;
  friend MenuFocusOverlay;
  
 public:
  
  // Public instance variables
  Scene*            scene;
  MenuInterface*    imgMenu;
  Camera            camera;

  // Public methods
  Window3D(int pixWidth, int pixHeight, const char* title);
  ~Window3D(void);
  void loop(void);
  bool diagnosticHTML(HttpDebug* serv);
  
  // Static class methods
  static bool HTTPGateway(HttpDebug* serv, char* path);
  static void terminate(void);
  static Window3D& getActiveWin(void);
  static Window3D& getMainWin(void);
  static void initGraphics(void);

 protected:

  // Instance variables - protected
  int             width;
  int             height;
  GLFWwindow*     window;
  const char*     winTitle;
  
  // Virtual interface that subclasses must implement.
  virtual void  draw(float timeInterval);
  virtual void  processClick(float mouseX, float mouseY);
  virtual void  processDoubleClick(float mouseX, float mouseY, float timeDiff);

  // Virtual interface that subclasses can optionally override.
  virtual void  processKeyboard(void);

 private:
  
  // Class variables - private
  
  // Instance variables - private
  double          lastMouseX, lastMouseY;
  Timeval         lastTime;
  Timeval         mouseUpTime;
  Timeval         clickTime;
  float           clickLength;
  bool            inClick;
  bool            testingDoubleClick;
  bool            mouseMoved;
  float           frameTimeAvg;
  int             ourWin;
  
  // Static private variables
  static Window3D*  theWin;   //ultimately will need to be container of all windows
  static int        nextWin;
  static int        activeWin;
  static std::unordered_map<int, Window3D*> windows;
  
  // Private methods
  void          processMouse(void);
  ActionType    processAction(InterfaceAction* action);
  float         timeDelta(void);           // number of microseconds since last call

  Window3D(const Window3D&);                 // Prevent copy-construction
  Window3D& operator=(const Window3D&);      // Prevent assignment

};

#endif
