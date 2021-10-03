// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -

#ifndef WINDOW3D_H
#define WINDOW3D_H
#include "Scene.h"
#include "Camera.h"
#include "MenuInterface.h"
#include "GLFWApplication.h"
#include <sys/time.h>
#include <GLFW/glfw3.h>


// =======================================================================================
// Forward declarations

class MenuFocusOverlay;
class HttpDebug;


// =======================================================================================
/// @brief Utility class for opening a window and setting up for drawing in it using 
/// OpenGL commands.
///
/// The present implementation uses GLFW but the API should hide that.  This
/// class does not support creating bare instances of itself, but instead should be 
/// subclassed to provide different types of window, which then requires overriding 
/// draw(), processClick(), processDoubleClick(), diagnosticHTMLRow(), and 
/// diagnosticHTML(). and optionally processKeyboard() and processAction().
/// 
/// This class provides the subclasses with the basics of OpenGL window creation, raw 
/// mouse/keyboard processing (eg camera drag, click/double-click detection), and first 
/// cut HTTP interface action processing.

class Window3D
{
  friend MenuPanel;
  friend MenuFocusOverlay;
  friend GLFWApplication;
  
 public:
  
  // Public instance variables
  Scene*            scene;
  Camera            camera;

  // Public methods
  Window3D(int pixWidth, int pixHeight, const char* title, GLFWApplication& app,
                                                            Window3D* existing = NULL);
  ~Window3D(void);
  void  loop(void);
  int   makeFocus(void);
  void  move(int x, int y);
  void  resize(int width, int height);
  int   initPanel(char* question, char** responses, int nResponses);
  
 protected:

  // Instance variables - protected
  int               width;
  int               height;
  GLFWwindow*       window;
  const char*       winTitle;
  GLFWApplication&  appParent;
  int               ourWin;
  bool              exitLoopRequired;
  
  // Virtual interface that subclasses must implement.
  virtual void  draw(float timeInterval);
  virtual void  processClick(float mouseX, float mouseY);
  virtual void  processDoubleClick(float mouseX, float mouseY, float timeDiff);
  virtual bool  diagnosticHTMLRow(HttpDebug* serv, int rowIndex);
  virtual bool  diagnosticHTML(HttpDebug* serv);

  // Virtual interface that subclasses can optionally override.
  virtual void        processKeyboard(void);
  virtual ActionType  processAction(InterfaceAction* action);

 private:
    
  // Instance variables - private
  double          lastMouseX, lastMouseY;
  Timeval         lastTime;
  Timeval         mouseUpTime;
  Timeval         clickTime;
  float           clickLength;
  bool            inClick;
  bool            testingDoubleClick;
  bool            mouseMoved;
    
  // Static private variables
  static bool GLFWInitDone;
  
  // Private methods
  void          processMouse(void);
  float         timeDelta(void);           // number of microseconds since last call

  Window3D(const Window3D&);                 // Prevent copy-construction
  Window3D& operator=(const Window3D&);      // Prevent assignment

};


// =======================================================================================

#endif
