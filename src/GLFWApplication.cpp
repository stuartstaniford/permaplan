// Copyright Staniford Systems.  All Rights Reserved.  September 2021 -
// This manages the overall rendering and simulation loop and keeps track of 
// which windows are currently open/on-screen.  It's very specific to OpenGL/GLFW.

#include "GLFWApplication.h"
#include "Window3D.h"
#include "Shader.h"


// =======================================================================================
// Static variables

GLFWApplication* GLFWApplication::theSingletonApp = NULL;


// =======================================================================================
/// @brief Constructor
///
/// Constructor should only be called once at startup.  Everyone else gets us via
/// getGLFWApplication()

GLFWApplication::GLFWApplication(void):
                                scene(NULL), // will be set later in main.cpp
                                frameTimeAvg(0.0f),
                                nextWin(0),
                                activeWin(-1),
                                frameCount(0),
                                firstTime(true)
{
  if(theSingletonApp)
    return;
  else
    theSingletonApp = this;
  
  initGraphics();
}


// =======================================================================================
/// @brief Destructor - do final cleanup of the window interface after all windows are 
/// done

GLFWApplication::~GLFWApplication(void)
{
  LogWindowOperations("Terminating window system altogether.\n");  
  glfwTerminate();
}


// =======================================================================================
/// @brief Callback used for when the GLFW library needs to report errors.

void errorCallbackForGLFW(int error, const char* description)
{
  LogGLFWErrors("Error: %s\n", description);
}


// =======================================================================================
/// @brief Initialize the graphics system.
/// 
/// Must be called early in the program before any other graphics calls are done.  
/// Currently called from the constructor.

void GLFWApplication::initGraphics(void)
{
  // Initialize GLFW and define version and compatibility settings
  glfwSetErrorCallback(errorCallbackForGLFW);
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
 }


// =======================================================================================
/// @brief Return the singleton instance of GLFWApplication

GLFWApplication& GLFWApplication::getGLFWApplication(void) 
{
  return *theSingletonApp;
}

// =======================================================================================
/// @brief Helper function to do various openGL logging

void GLFWApplication::openGLInitialLogging(void)
{
#ifdef LOG_OPENGL_CONSTANTS
  int params[4];
  
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, params);
  LogOpenGLConstants("OpenGL Max texture size is %d.\n", params[0]);

/* This next causes an OpenGL error for some reason, so commented out.
 glGetIntegerv(GL_MAX_TEXTURE_UNITS, params);
  LogOpenGLConstants("OpenGL Max texture units is %d.\n", params[0]);
*/
  
  if(checkGLError(stderr, "openGLInitialLogging"))
    exit(-1);
#endif
}


// =======================================================================================
/// @brief Measure and log the inter-frame timing.
/// 
/// This method is called from within the loop of the curently active window, and has
/// the job of measuring how long it is from one frame to the next, and logging that if 
/// necessary.
/// @param callingWindow The Window3D that invoked the function. 

void GLFWApplication::measureFrameTiming(Window3D* callingWindow)
{  
  frameTime.now();
  frameDouble = frameTime - start;  // in seconds
  if(firstTime)
   {
    LogFrameStarts("Frame %u starting at %.6lfs\n", frameCount, frameDouble);
    firstTime = false;
   }
  else
   {
    LogFrameStarts("Frame %u starting in %s at %.6lfs (%.1fms gap, year %.2f)\n",
                 frameCount, callingWindow->winTitle, frameDouble, 
                 (frameDouble - lastFrameDouble)*1000.0f, scene->getSimYear());
    if(frameTimeAvg == 0.0f)
      frameTimeAvg = frameDouble - lastFrameDouble;
    else
      frameTimeAvg = 0.001f*(frameDouble - lastFrameDouble) + 0.999f*frameTimeAvg;
   }
}


// =======================================================================================
/// @brief Final book-keeping for this spin of the loop
/// 
/// This method is called from within the loop of the curently active window, and has
/// the job of updating variables at the end of the current spin of the rendering loop.
/// @param callingWindow The Window3D that invoked the function. 

void GLFWApplication::finalFrameBookkeeping(Window3D* callingWindow)
{
  lastFrameDouble = frameDouble;
  frameCount++;
}


// =======================================================================================
/// @brief Make us the target for OpenGL commands and also the focus window for keyboard
/// and mouse events. 
///
/// This is called from GLFWApplication::switchFocus, which has to handle tracking in
/// it's data structures that we are now the active window.  Note this link has some very 
/// helpful discussion and background for the issues this
/// function has to handle:
/// https://gregnott.wordpress.com/2013/05/18/tutorial-multiple-windows-with-glfw3-and-glew-mx/.
/// @returns The integer value of ourWin (our index in the GLFWApplication data 
/// structures.

void GLFWApplication::switchFocus(Window3D* window)
{
  lock();
  int ourWin = window->makeFocus();
  activeWin = ourWin;
  unlock();
}


// =======================================================================================
/// @brief Add a new window to the application to be managed

void GLFWApplication::addWindow(Window3D& window)
{
  window.scene = scene;
}


// =======================================================================================
/// @brief Make us the next window to run (and cause the current window loop to stop at
/// the end of the frame.
/// @param caller A pointer to the current window who is calling this function to make
/// some other window the next to be focus.

void GLFWApplication::scheduleWindowNext(Window3D* caller, Window3D* nextWindow)
{
  lock();
  assert(nextWindow->ourWin != activeWin);  // shouldn't be rescheduling ourself
  windowStack.push_back(nextWindow->ourWin);
  unlock();
  caller->exitLoopRequired = true;  // caller giving up control of the display loop 
                                    // at the end of this cycle.
}


// =======================================================================================
/// @brief Function to co-ordinate the loops of the various windows.
/// 
/// This ensures that only one is executing at a time (to avoid deadlocks), and to manage 
/// the transition when one ends and another starts.

void GLFWApplication::overLoop(void)
{
  lock();
  windowStack.push_back(0);
  unlock();
  start.now();
  
  while(1)
   {
    Window3D* win = NULL;
    lock();
    if(windowStack.size())
     {
      int& nextWinToRun = windowStack.back();
      windowStack.pop_back();
      win = windows[nextWinToRun];
      unlock();
     }
    else
     {
      unlock();
      break;      
     }
    switchFocus(win);
    frameTime.now();
    LogWindowOperations("Entering window loop for %s after %.6lf\n", 
                                                      win->winTitle, frameTime - start);
    win->loop();
    frameTime.now();
    LogWindowOperations("Orderly exit from window loop for %s after %.6lf\n", 
                                                      win->winTitle, frameTime - start);
   }
  LogCloseDown("Orderly exit from GLFWApplication::overLoop after %.6lf\n", frameTime - start);
}


// =======================================================================================
/// @brief Tunction to get a reference to the active window
/// @returns The reference

Window3D& GLFWApplication::getActiveWin(void)
{
  lock();
  Window3D& theWin = *(windows[activeWin]);
  unlock();
  return theWin;  
}


// =======================================================================================
/// @brief Function to get a reference to the main scene window that's open from
/// the beginning
/// @returns The reference

Window3D& GLFWApplication::getMainWin(void)
{
  return *(windows[0]);  
}


// =======================================================================================
/// @brief Function called in the HTTP server thread to list all open windows.  
/// 
/// This is here, not in Window3D, because it lists all windows, not just a particular 
/// instance.
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool GLFWApplication::HTTPListActiveWindows(HttpDebug* serv)
{
  unless(serv->startResponsePage("Open Windows"))
    return false;

  // Beginning of table  
  httPrintf("<center>\n");
  unless(serv->startTable((char*)"windowList"))
    return false;
  httPrintf("<tr><th>Index</th><th>Name</th><th>Details</th></tr>\n");
  
  // Iterate over the rows
  int row = 1;
  lock();
  for (auto& winIter: windows)
    winIter.second->diagnosticHTMLRow(serv, row++);
  unlock();
  
  // End the table, the page, and go home.
  httPrintf("</table></center><hr>\n");
  unless(serv->endResponsePage())
    return false;
  return true;
}


// =======================================================================================
/// @brief Function called in the HTTP server thread to handle /window/ API calls.  
/// 
/// This is here, rather than in Window3D, because this action has not been associated 
/// with a particular window yet.
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool GLFWApplication::HTTPGateway(HttpDebug* serv, char* path)
{  
  if(strncmp(path, "list/", 5)== 0)
   {
    return HTTPListActiveWindows(serv);
   }
  if(strncmp(path, "detail/", 7)== 0)
   {
    int digitSize = strlen(path+7);
    unless(digitSize > 0 && digitSize < 5)
     {
      LogRequestErrors("GLFWApplication::HTTPGateway detail/ path invalid: %s\n", path);
      return false;
     }
    int winIndex = atoi(path+7);
    unless(windows.count(winIndex))
     {
      LogRequestErrors("GLFWApplication::HTTPGateway detail/ index %d not found.\n", winIndex);
      return false;
     }
    return windows[winIndex]->diagnosticHTML(serv);
   }
  else if(strncmp(path, "resize/", 7)== 0)
   {
    InterfaceAction* action = new InterfaceAction(WindowResize, path+7);
    if(action->valid)
     {
      lock();
      windows[activeWin]->scene->actions.push_back(action);
      unlock();

      httPrintf("OK\n");
      return true;
     }
    else
     {
      LogRequestErrors("GLFWApplication::HTTPGateway couldn't create valid resize action"
                                                                      " from %s\n", path);
      return false;
     }
   }
  else if(strncmp(path, "move/", 5)== 0) //XX this API will need rework for multiple windows
   {
    InterfaceAction* action = new InterfaceAction(WindowMove, path+5);
    if(action->valid)
     {
      lock();
      windows[activeWin]->scene->actions.push_back(action);
      unlock();
      httPrintf("OK\n");
      return true;
     }
    else
     {
      LogRequestErrors("GLFWApplication::HTTPGateway couldn't create valid move action"
                                                                      " from %s\n", path);
      return false;
     }
   }
  
  LogRequestErrors("GLFWApplication::HTTPGateway unknown directive %s\n", path);
  return false;
}


// =======================================================================================
