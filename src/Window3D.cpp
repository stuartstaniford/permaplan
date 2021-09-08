// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Utility class for opening a window and setting up for drawing in using
// OpenGL commands.  The present implementation uses GLFW but the API should
// hide that.

#include "Window3D.h"
#include "PmodException.h"
#include "Material.h"
#include "Species.h"
#include "MenuInsert.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <cglm/cglm.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


// =======================================================================================
// File level and static variables

bool GLFWInitDone = false;   // For overall openGL initialization

Lockable Window3D::staticWindowLock;
std::unordered_map<int, Window3D*> Window3D::windows;   
int Window3D::nextWin = 0;
int Window3D::activeWin = -1;
std::vector<int> Window3D::windowStack;
unsigned Window3D::frameCount = 0;
Timeval Window3D::start;
Timeval Window3D::frameTime;
bool Window3D::firstTime = true;


// =======================================================================================
/// @brief Calback for window resizing in GLFW

void windowResize(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}


// =======================================================================================
/// @brief Helper function to do various openGL logging

void openGLInitialLogging(void)
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
/// @brief Callback used for when the GLFW library needs to report errors.

void glfwErrorCallback(int error, const char* description)
{
  LogGLFWErrors("Error: %s\n", description);
}


// =======================================================================================
/// @brief Static function to initialize the graphics system.
/// 
/// Must be called early in the program before any other graphics calls are done.  The
/// file level variable GLFWInitDone ensures we only do it once.

void Window3D::initGraphics(void)
{
  // Initialize GLFW and define version and compatibility settings
  glfwSetErrorCallback(glfwErrorCallback);
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
 }


// =======================================================================================
/// @brief Constructor for a Window3D
///
/// See https://learnopengl.com/Getting-started/Hello-Window.
/// Note that Window3D::initGraphics must be called before the first Window3D can be 
/// called.
/// @param pixWidth The number of pixels wide for the window
/// @param pixHeight The number of pixels high for the window
/// @param existing A pointer to an existing window, with which this new window should
/// share an OpenGL context.  This is NULL by default (creating a new context).

Window3D::Window3D(int pixWidth, int pixHeight, const char* title, Window3D* existing):
                        camera(200.0f, 45.0f),
                        width(pixWidth),
                        height(pixHeight),
                        winTitle(title),
                        exitLoopRequired(false),
                        lastMouseX(HUGE_VAL),
                        lastMouseY(HUGE_VAL),
                        inClick(false),
                        testingDoubleClick(false),
                        mouseMoved(true),
                        frameTimeAvg(0.0f)
{  
  unless(GLFWInitDone)
    Window3D::initGraphics();
  
  // Use GLFW to create OpenGL window and context
  if(existing)
    window = glfwCreateWindow(width, height, title, NULL, existing->window);
  else
    window = glfwCreateWindow(width, height, title, NULL, NULL);    
  if (!window)
    err(-1, "Couldn't create window %s.\n", title);

  unless(GLFWInitDone)
    glfwMakeContextCurrent(window);

  unless(GLFWInitDone)
   { 
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewResult = glewInit();
    if (GLEW_OK != glewResult)
      err(2, "glewInit failed.\n");
    // Perform logging of some features of the OpenGLimplementation (if log types on)
#ifdef LOG_OPENGL_CONSTANTS
    openGLInitialLogging();
#endif
    GLFWInitDone = true;
   }

  // Set up the OpenGL Viewport
  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, windowResize);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glfwSwapInterval(1);

  glViewport(0, 0, width, height);
  lastTime.now();

  staticWindowLock.lock();
  ourWin           = nextWin++;
  windows[ourWin]  = this;
  staticWindowLock.unlock();
}


// =======================================================================================
/// @brief Destructor for any window.

Window3D::~Window3D(void)
{
  LogWindowOperations("Deallocating %s window at %.2f.\n", winTitle, frameTime-start);  
  staticWindowLock.lock();
  if(activeWin == ourWin)
    activeWin = 0u;
  staticWindowLock.unlock();
  windows.erase(ourWin);
  glfwDestroyWindow(window);
}


// =======================================================================================
/// @brief Static function to do final cleanup of the window interface after all 
/// windows are done.

void Window3D::terminate()
{
  LogWindowOperations("Terminating window system altogether.\n");  
  glfwTerminate();
}


// =======================================================================================
/// @brief Move this window to a new location on the screen.

void Window3D::move(int x, int y)
{
  glfwSetWindowPos(window, x, y);  
  LogWindowOperations("Window moved to screen coordinates %d, %d.\n", x, y);  
}


// =======================================================================================
/// @brief Resize this window.

void Window3D::resize(int width, int height)
{
  glfwSetWindowSize(window, width, height);  
  LogWindowOperations("Window resized to width %d, height %d.\n", width, height); 
}


// =======================================================================================
/// @brief Handle an pseudo-interface event coming from the HTTP Debug interface.  
/// 
/// We get these first, and our job is to siphon off those actions that require some
/// kind of window operation.  Subclasses should override this, but then call it first
/// in their overriden version to take care of the cases we handle.
/// @returns The ActionType of the action we processed.  If the action was not one of 
/// ours, we return NoAction.
/// @param action The InterfaceAction that needs to be handled.

ActionType Window3D::processAction(InterfaceAction* action)
{  
  switch(action->actionType)
   {
    case CameraMoved:
      camera.updateAfterMove();
      return CameraMoved;
      
    case Click:
      // for Click and DoubleClick, the data is mousePos
      LogPseudoActions("Click processed at %.1f, %.1f.\n", action->data[0], 
                                                                  action->data[1]);
      processClick(action->data[0], action->data[1]);
      return Click;
      
    case DoubleClick:
      LogPseudoActions("Double-click processed at %.1f, %.1f.\n", action->data[0], 
                                                                  action->data[1]);
      processDoubleClick(action->data[0], action->data[1], 0.1f);
      return DoubleClick;

    case QuitProgram:
      LogPseudoActions("Quit program processed.\n");
      return QuitProgram;  // handled in our caller loop()

    case WindowMove:
      LogPseudoActions("Window move action to %.0f, %.0f processed.\n",
                                              action->data[0], action->data[1]);
      move((int)action->data[0], (int)action->data[1]);
      return WindowMove;

    case WindowResize:
      resize((int)action->data[0], (int)action->data[1]);
      LogPseudoActions("Window resize action to width %d, height %d processed.\n",
                                      (int)action->data[0], (int)action->data[1]); 
      return WindowResize;

    default:
      return NoAction;
   }
}


// =======================================================================================
/// @brief Make us the target for OpenGL commands and also the focus window for keyboard
/// and mouse events. 
///
/// Note this link has some very helpful discussion and background for the issues this
/// function has to handle:
/// https://gregnott.wordpress.com/2013/05/18/tutorial-multiple-windows-with-glfw3-and-glew-mx/.

void Window3D::makeFocus(void)
{
  //glfwMakeContextCurrent(window);
  Shader::validateShader((char*)"In makeFocus");
  glfwFocusWindow(window);  
  staticWindowLock.lock();
  activeWin = ourWin;
  staticWindowLock.unlock();
}


// =======================================================================================
/// @brief Make us the next window to run (and cause the current window loop to stop at
/// the end of the frame.
/// @param caller A pointer to the current window who is calling this function to make
/// some other window the next to be focus.

void Window3D::scheduleWindowNext(Window3D* caller)
{
  staticWindowLock.lock();
  assert(ourWin != activeWin);  // shouldn't be rescheduling ourself
  windowStack.push_back(ourWin);
  staticWindowLock.unlock();
  caller->exitLoopRequired = true;  // caller giving up control of the display loop 
                                    // at the end of this cycle.
}


// =======================================================================================
/// @brief Static function to co-ordinate the loops of the various windows.
/// 
/// This ensures that only one is executing at a time (to avoid deadlocks), and to manage 
/// the transition when one ends and another starts.

void Window3D::overLoop(void)
{
  staticWindowLock.lock();
  windowStack.push_back(0);
  staticWindowLock.unlock();
  
  while(1)
   {
    Window3D* win = NULL;
    staticWindowLock.lock();
    if(windowStack.size())
     {
      int& nextWinToRun = windowStack.back();
      windowStack.pop_back();
      win = windows[nextWinToRun];
      staticWindowLock.unlock();
     }
    else
     {
      staticWindowLock.unlock();
      break;      
     }
    win->makeFocus();
    win->loop();
   }
  LogCloseDown("Orderly exit from Window3D::overLoop after %.6lf\n", frameTime - start);
}


// =======================================================================================
/// @brief Event processing loop for our window.
///
/// @todo We are not distinguishing between loop exits from the user closing the window
/// and loop exits because we are going to switch to another window.  In the former case
/// we are supposed to deallocate the window and close it.
/// https://www.glfw.org/docs/latest/quick.html#quick_window_close
/// @todo We are not correctly using framebuffer size vs window size in the loop. 
/// https://www.glfw.org/docs/latest/window_guide.html#window_size
/// https://www.glfw.org/docs/latest/window_guide.html#window_fbsize

void Window3D::loop(void)
{
  double    frameDouble;
  double    lastFrameDouble;

  makeFocus();
  if(firstTime)
    start.now();
  frameTime.now();
  LogWindowOperations("Entering window loop for %s after %.6lf\n", winTitle,
                                                                        frameTime - start);
  // Main event loop
  while(!glfwWindowShouldClose(window))
   {
    // Timing measurements and logging
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
                     frameCount, winTitle, frameDouble, 
                     (frameDouble - lastFrameDouble)*1000.0f, scene->getSimYear());
      if(frameTimeAvg == 0.0f)
        frameTimeAvg = frameDouble - lastFrameDouble;
      else
        frameTimeAvg = 0.001f*(frameDouble - lastFrameDouble) + 0.999f*frameTimeAvg;
     }
    camera.logFrames();
    
    // OpenGL calls to clear buffer
    glClearColor(0.6f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwGetWindowSize(window, &width, &height); // make sure we know current size
    
    if(checkGLError(stderr, "Window3D::loop before scene lock."))
      exit(-1);

    scene->lock();

    // Do our actual drawing and deliver to screen window
    draw((float)(frameDouble - lastFrameDouble));
    glfwSwapBuffers(window);
    
    // Process pseudo-IO from HTTP interface
    InterfaceAction* action = NULL;
    while(int n = scene->actions.size())
     {
       action = scene->actions[n-1];
       if(processAction(action) == QuitProgram)
         break;
       scene->actions.pop_back();
       delete action;
     }
    if(action && action->actionType == QuitProgram)
      break;
    
    // Process IO
    ImGuiIO& io = ImGui::GetIO();
    glfwPollEvents();
    unless(io.WantCaptureMouse)
     {
      processKeyboard();
      processMouse();
     }
    else
      LogMouseLocation("ImGui has mouse\n");

    scene->unlock();

    // Final book-keeping for this spin of the loop
    lastFrameDouble = frameDouble;
    frameCount++;
    if(exitLoopRequired)
     {
      exitLoopRequired = false;
      break;
     }
   }
  
  // Shutdown stuff after main event loop is done.
  frameTime.now();
  LogWindowOperations("Orderly exit from window loop for %s after %.6lf\n", winTitle,
                                                                        frameTime - start);
}


// =======================================================================================
/// @brief Interface for the window specific rendering action.  This should be overriden
/// by our subclasses.

void Window3D::draw(float timeInterval)
{
  err(-1, "Shouldn't call superclass method Window3D::draw.\n");
}


// =======================================================================================
/// @brief Interface for the window specific handling of a mouse click.  This should be 
/// overriden by our subclasses.
/// @param mouseX X location of mouse click in window co-ordinates
/// @param mouseY Y location of mouse click in window co-ordinates

void  Window3D::processClick(float mouseX, float mouseY)
{
  err(-1, "Shouldn't call superclass method Window3D::processClick.\n");
}


// =======================================================================================
/// @brief Interface for the window specific handling of a double-click in the window, 
/// This should be overriden by our subclasses.
/// @param mouseX X location of mouse click in window co-ordinates
/// @param mouseY Y location of mouse click in window co-ordinates
/// @param timeDiff the interval between the two clicks in seconds

void Window3D::processDoubleClick(float mouseX, float mouseY, float timeDiff)
{
  err(-1, "Shouldn't call superclass method Window3D::processDoubleClick.\n");
}


// =======================================================================================
/// @brief Handle mouse input in the window.
/// 
/// This function is the first level handling of the mouse when it's in our window.  This
/// generally would not be overriden, but we call methods like processClick and 
/// processDoubleClick that deal with those events on a subclass-specific basis.
/// @todo Need to handle case of clicking in an inactive window.

void Window3D::processMouse(void)
{
  Timeval justNow;
  float clickSpacing;
  
  if (!glfwGetWindowAttrib(window, GLFW_HOVERED))
   {
    LogMouseLocation("Mouse cursor out of window\n");
    return; // cursor is not with us, nothing to do.
   }
  
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);

  if( (abs(mouseX - lastMouseX) < EPSILON) && (abs(mouseY - lastMouseY) < EPSILON))
   {
    mouseMoved = false;
    LogMouseLocation("Mouse unmoved at %.1f, %.1f\n", mouseX, mouseY);
   }
  else
   {
    mouseMoved = true;
    LogMouseLocation("Mouse moved to %.1f, %.1f (by %.1f,%.1f)\n", mouseX, mouseY,
                     mouseX - lastMouseX, mouseY - lastMouseY);
   }
  
  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
  
  if (state == GLFW_PRESS) // mouse button is pressed
   {
    if(!inClick)  // it *just* got pressed and this is our first frame processing that fact
     {
      clickTime.now();
      LogClickDetails("Mouse press started at %s", clickTime.ctimeString());
      inClick = true;
      if(testingDoubleClick)
       {
        justNow.now();
        clickSpacing = justNow - mouseUpTime;
        LogClickDetails("Testing for double-click after %.3f.\n", clickSpacing);
        if(clickSpacing < 0.25)
         {
          testingDoubleClick = false;
          processDoubleClick((float)mouseX, (float)mouseY, clickSpacing);
         }
       }
     }
    else if(mouseMoved) // drag the camera
     {
      double xDelta = (mouseX - lastMouseX)/width;
      double yDelta = (mouseY - lastMouseY)/height;
      if(xDelta < -1.0 || xDelta > 1.0 || yDelta < -1.0 || yDelta > 1.0)
        goto processMouseExit; // weird, don't know what to do.
      LogClickDetails("Mouse move detected by %.4lf,%.4lf.\n", xDelta, yDelta);
      camera.mouseDrag((float)xDelta, (float)yDelta);
     }
   }
  else if(inClick)  // mouse button must have just been released
   {
    inClick            = false;
    testingDoubleClick = true;
    mouseUpTime.now();
    clickLength = mouseUpTime - clickTime;
    LogClickDetails("Mouse lifted after click length of %.3f at %lu from %lu.\n", clickLength,
                    mouseUpTime.tv_sec, clickTime.tv_sec);
   }
  else if(testingDoubleClick) // see if we timed out on double click detection
   {
    justNow.now();
    clickSpacing = justNow - mouseUpTime;
    if(clickSpacing > 0.25)
     {
      LogClickDetails("Timing out double-click detection after %.3f.\n", clickSpacing);
      // This isn't a double click, so see if it's a single click.
      testingDoubleClick = false;
      if(clickLength < 0.2) // don't treat drags as a click
        processClick((float)mouseX, (float)mouseY);
     }
   }

processMouseExit:
  lastMouseX = mouseX;
  lastMouseY = mouseY;
}


// =======================================================================================
/// @brief Handle keyboard input in the window
///
/// This will work for some subclasses, but some may wish to override.
// Tristan comments: WASD is forward/back and lateral side-to-side.  Mouse movement is 
// looking around.  Minecraft has walk/fly mode, space jumps up into the air, but still 
// follows ground contour.  E is a good button for interaction - convenient to WASD.

void Window3D::processKeyboard(void)
{
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, 1);
  else
   {
    unsigned camOpFlags = 0u;

    // The arrow keys are used to control pitch and yaw of the camera direction
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      camOpFlags |= CAM_PITCH_UP;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      camOpFlags |= CAM_PITCH_DOWN;
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
      camOpFlags |= CAM_YAW_LEFT;
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
      camOpFlags |= CAM_YAW_RIGHT;
    
    // A and D keys control camera moving left and right
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camOpFlags |= CAM_MOVE_LEFT;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camOpFlags |= CAM_MOVE_RIGHT;

    // W and S keys move the camera forward and backwards
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      camOpFlags |= CAM_MOVE_FORWARD;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      camOpFlags |= CAM_MOVE_BACK;

    // Shift and space-shift move the camera up and down
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
     {
      if((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
          || (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS))
        camOpFlags |= CAM_MOVE_DOWN;
      else
        camOpFlags |= CAM_MOVE_UP;
      
     }
    if(camOpFlags)
     {
      float delta = timeDelta();
      camera.adjust(camOpFlags, delta);
     }
   }
}


// =======================================================================================
/// @brief Returns the number of microseconds since last call

float Window3D::timeDelta(void)
{
  Timeval thisMoment;
  thisMoment.now();
  double diff = (thisMoment - lastTime)*1e6;
  lastTime = thisMoment;
  
  return (float)diff;
}


// =======================================================================================
/// @brief Static function to get a reference to the active window
/// @returns The reference

Window3D& Window3D::getActiveWin(void)
{
  staticWindowLock.lock();
  Window3D& theWin = *(windows[activeWin]);
  staticWindowLock.unlock();
  return theWin;  
}


// =======================================================================================
/// @brief Static function to get a reference to the main scene window that's open from
/// the beginning
/// @returns The reference

Window3D& Window3D::getMainWin(void)
{
  return *(windows[0]);  
}


// =======================================================================================
/// @brief Function called in the HTTP server thread to provide a table row about this
/// window.  
/// 
/// This needs to be overridden by subclasses and the implementation in Window3D will 
/// error-exit.  Subclasses should provide a link of the form /window/detail/%d/ to 
/// their detail page, which in turn should be provided by diagnosticHTML().
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool Window3D::diagnosticHTMLRow(HttpDebug* serv, int rowIndex)
{
  err(-1, "Call to unimplemented superclass Window3D::diagnosticHTMLRow");
}


// =======================================================================================
/// @brief Function called in the HTTP server thread to list all open windows.  
/// 
/// This is a static method, because it lists all windows, not just a particular instance.
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool Window3D::HTTPListActiveWindows(HttpDebug* serv)
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
  staticWindowLock.lock();
  for (auto& winIter: windows)
    winIter.second->diagnosticHTMLRow(serv, row++);
  staticWindowLock.unlock();
  
  // End the table, the page, and go home.
  httPrintf("</table></center><hr>\n");
  unless(serv->endResponsePage())
    return false;
  return true;
}


// =======================================================================================
/// @brief Function called in the HTTP server thread to handle /window/ API calls.  
/// 
/// This is a static method, because this action has not been associated with a
/// particular window yet.
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool Window3D::HTTPGateway(HttpDebug* serv, char* path)
{  
  if(strncmp(path, "list/", 5)== 0)
   {
    return Window3D::HTTPListActiveWindows(serv);
   }
  if(strncmp(path, "detail/", 7)== 0)
   {
    int digitSize = strlen(path+7);
    unless(digitSize > 0 && digitSize < 5)
     {
      LogRequestErrors("Window3D::HTTPGateway detail/ path invalid: %s\n", path);
      return false;
     }
    int winIndex = atoi(path+7);
    unless(windows.count(winIndex))
     {
      LogRequestErrors("Window3D::HTTPGateway detail/ index %d not found.\n", winIndex);
      return false;
     }
    return windows[winIndex]->diagnosticHTML(serv);
   }
  else if(strncmp(path, "resize/", 7)== 0)
   {
    InterfaceAction* action = new InterfaceAction(WindowResize, path+7);
    if(action->valid)
     {
      staticWindowLock.lock();
      windows[activeWin]->scene->actions.push_back(action);
      staticWindowLock.unlock();

      httPrintf("OK\n");
      return true;
     }
    else
     {
      LogRequestErrors("Window3D::HTTPGateway couldn't create valid resize action"
                                                                      " from %s\n", path);
      return false;
     }
   }
  else if(strncmp(path, "move/", 5)== 0) //XX this API will need rework for multiple windows
   {
    InterfaceAction* action = new InterfaceAction(WindowMove, path+5);
    if(action->valid)
     {
      staticWindowLock.lock();
      windows[activeWin]->scene->actions.push_back(action);
      staticWindowLock.unlock();
      httPrintf("OK\n");
      return true;
     }
    else
     {
      LogRequestErrors("Window3D::HTTPGateway couldn't create valid move action"
                                                                      " from %s\n", path);
      return false;
     }
   }
  
  LogRequestErrors("Window3D::HTTPGateway unknown directive %s\n", path);
  return false;
}


// =======================================================================================
/// @brief Function called in the HTTP server thread to provide a whole page about this
/// window.  
/// 
/// This needs to be overridden by subclasses and the implementation in Window3D will 
/// error-exit.  Subclasses should provide a summary page of any/all detail that would
/// be useful for debugging/testing when an instance of their window is open.
/// @returns True if the HeightMarker was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool Window3D::diagnosticHTML(HttpDebug* serv)
{
  err(-1, "Call to unimplemented superclass Window3D::diagnosticHTML");
  
  return true;
}


// =======================================================================================
