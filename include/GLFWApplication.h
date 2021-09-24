// Copyright Staniford Systems.  All Rights Reserved.  September 2021 -

#ifndef GLFW_APPLICATION_H
#define GLFW_APPLICATION_H

#include "Lockable.h"
#include "Timeval.h"
#include <unordered_map>
#include <vector>


// =======================================================================================
// Forward declarations

class Window3D;
class HttpDebug;
class Scene;


// =======================================================================================
/// @brief Overall organization of an application consisting of multiple GLFW Windows.
/// 
/// This manages the overall rendering and simulation loop and keeps track of which 
/// windows are currently open/on-screen.  It's very specific to OpenGL/GLFW.  It 
/// inherits from Lockable and the purpose of this lock is to protect the data structures
/// that keep track of the list of windows, which window is active, and so on.

class GLFWApplication: public Lockable
{
  friend Window3D;
  
public:
  
  // Instance variables - public
  Scene*  scene;
  /// @brief Estimate the average time between successive frames.
  float frameTimeAvg;

  // Member functions - public
  GLFWApplication(void);
  ~GLFWApplication(void);
  void addWindow(Window3D& window);

  bool HTTPGateway(HttpDebug* serv, char* path);
  bool HTTPListActiveWindows(HttpDebug* serv);
  void terminate(void);
  Window3D& getActiveWin(void);
  Window3D& getMainWin(void);
  void initGraphics(void);
  void overLoop(void);
  void openGLInitialLogging(void);
  void switchFocus(Window3D* window);
  void scheduleWindowNext(Window3D* caller, Window3D* nextWindow);
  void measureFrameTiming(Window3D* callingWindow);
  void finalFrameBookkeeping(Window3D* callingWindow);

  // Static functions public
  static GLFWApplication& getGLFWApplication(void); 
  
private:
  
  // Instance variables - private
  
  /// @brief The next unused id.
  int        nextWin;
  /// @brief The id of the currently active window
  int        activeWin;
  ///@brief Keep track of all open windows via an id.
  std::unordered_map<int, Window3D*> windows;
  ///@brief A list of windows to make active next.
  std::vector<int> windowStack;
  /// @brief The index of the current frame in the loop.  Does not need the lock
  /// as only one loop can be running at a time.
  unsigned frameCount;
  /// @brief The time of the first frame in the render loop.  Does not need the lock
  /// as only one loop can be running at a time.
  Timeval start;
  /// @brief The time of the current frame in the loop.  Does not need the lock
  /// as only one loop can be running at a time.
  Timeval frameTime;
  /// @brief Used to identify the first run through of the render loop.  Does not 
  /// need the lock as only one loop can be running at a time.
  bool  firstTime;
  /// @brief Frame time as a distance from the start, in seconds
  double    frameDouble;
  /// @brief The frameDouble from the last spin of the loop.
  double    lastFrameDouble;

  // Static variables private
  static GLFWApplication* theSingletonApp;
  
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  GLFWApplication(const GLFWApplication&);       
  /// @brief Prevent assignment.
  GLFWApplication& operator=(const GLFWApplication&);      

};

#endif




