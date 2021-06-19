// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Utility class for opening a window and setting up for drawing in using
// OpenGL commands.  The present implementation uses GLFW but the API should
// hide that.

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unistd.h>
#include <cglm/cglm.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "Window3D.h"
#include "PmodException.h"
#include "Material.h"
#include "Species.h"


// =======================================================================================
// File level and static variables

bool GLFWInitDone = false;   // For overall openGL initialization

//ultimately will need to be container of all windows
Window3D* Window3D::theWin = NULL;   

// =======================================================================================
// Calback for window resizing

void windowResize(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}


// =======================================================================================
// Helper function to do various openGL logging

#ifdef LOG_OPENGL_CONSTANTS

void openGLInitialLogging(void)
{
  int params[4];
  
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, params);
  LogOpenGLConstants("OpenGL Max texture size is %d.\n", params[0]);

/* This next causes an OpenGL error for some reason, so commented out.
 glGetIntegerv(GL_MAX_TEXTURE_UNITS, params);
  LogOpenGLConstants("OpenGL Max texture units is %d.\n", params[0]);
*/
  
  if(checkGLError(stderr, "openGLInitialLogging"))
    exit(-1);
}

#endif

// =======================================================================================
// Constructor for a Window3D
// See https://learnopengl.com/Getting-started/Hello-Window

Window3D::Window3D(int pixWidth, int pixHeight):
                        scene(NULL),
                        scriptController(NULL),
                        width(pixWidth),
                        height(pixHeight),
                        lastMouseX(HUGE_VAL),
                        lastMouseY(HUGE_VAL),
                        inClick(false),
                        testingDoubleClick(false),
                        mouseMoved(true),
                        frameTimeAvg(0.0f)
{
  if(!GLFWInitDone)
   {
    // Initialize GLFW and define version and compatibility settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWInitDone = true;
   }
  
  // Use GLFW to create OpenGL window and context
  window = glfwCreateWindow(width, height, "Test", NULL, NULL);
  if (!window)
    throw std::runtime_error("Couldn't create window.");
    //throw pmodGLFWException("Couldn't create window.");
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  GLenum glewResult = glewInit();
  if (GLEW_OK != glewResult)
    err(2, "glewInit failed.\n");

  // Set up the OpenGL Viewport
  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, windowResize);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
 
  // Perform logging of some features of the OpenGLimplementation (if log types on)
#ifdef LOG_OPENGL_CONSTANTS
  openGLInitialLogging();
#endif

  // Dear ImGui initialization
  imgMenu = new MenuInterface(window, *this);
  glViewport(0, 0, width, height);  
  lastTime.now();
  
  theWin = this; //XX will not support multiple windows at present
}


// =======================================================================================
// Handle an pseudo-interface event coming from the HTTP Debug interface.
// ActionTypes are listed in alphabetical order

ActionType Window3D::processPseudoAction(InterfaceAction* action)
{
  const char* stringValue;
  
  switch(action->actionType)
   {
     case AllTreeSelection:
       // Warning stringValue is unprocessed except for being <=128
       stringValue = ((SpeciesPath*)(action->otherData))->getPath();
       LogPseudoActions("All tree selection %s processed.\n", stringValue);
       imgMenu->allTreeSelectorPseudoAction(stringValue);
       return AllTreeSelection;

     case BlockEntered:
      LogPseudoActions("Block entered size %.1f, material Strawbale.\n", action->data[0]);
      imgMenu->blockEnteredButton(action->data[0], "StrawBale");
      return BlockEntered;

    case CameraMoved:
      scene->camera.updateAfterMove();
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

    case HeightEntered:
      LogPseudoActions("Height of %.1f entered.\n", action->data[0]);
      imgMenu->heightEnteredButton(action->data[0]);
      return HeightEntered;

    case InsertBlock:
      LogPseudoActions("Insert block button press processed.\n");
      imgMenu->insertBlockButton();
      return InsertBlock;

    case InsertHeight:
      LogPseudoActions("Insert height button press processed.\n");
      imgMenu->insertHeightButton();
      return InsertHeight;       
       
    case InsertTree:
      LogPseudoActions("Insert tree button press processed.\n");
      imgMenu->insertTreeButton();
      return InsertTree;
      
    case QuitProgram:
      LogPseudoActions("Quit program processed.\n");
      return QuitProgram;  // handled in our caller loop()

    case SelectGenus:
      stringValue = ((SpeciesPath*)(action->otherData))->getPath();
      LogPseudoActions("Genus selection of %s processed.\n", stringValue);
      imgMenu->imguiTreeMenuButtonPressed(stringValue);
      return SelectGenus;
      
    case SimulatePause:
      LogPseudoActions("Pause button press processed.\n");
      LogSimulationControls("Pause Button \xe2\x8f\xb8 pressed.\n");
      scene->pauseSimulation();
      return SimulatePause;
       
    case SimulateReset:
      LogPseudoActions("Reset button press processed.\n");
      LogSimulationControls("Reset Button \xe2\x8f\xae pressed.\n");
      scene->resetSimulation();
      return SimulateReset;

    case SimulateStart:
      LogPseudoActions("Simulate start button press processed.\n");
      LogSimulationControls("Simulate Button \xe2\x96\xb6 pressed.\n");
      scene->startSimulation();
      return SimulateStart;

    case SimulateSpring:
      LogPseudoActions("Simulate spring button press processed.\n");
      LogSimulationControls("Spring season selected.\n");
      displaySeason = 0u;
      return SimulateSpring;

    case SimulateSummer:
      LogPseudoActions("Simulate summer button press processed.\n");
      LogSimulationControls("Summer season selected.\n");
      displaySeason = 1u;
      return SimulateSummer;
  
    case SimulateFall:
      LogPseudoActions("Simulate fall button press processed.\n");
      LogSimulationControls("Fall season selected.\n");
      displaySeason = 2u;
      return SimulateFall;

    case SimulateWinter:
      LogPseudoActions("Simulate winter button press processed.\n");
      LogSimulationControls("Winter season selected.\n");
      displaySeason = 3u;
      return SimulateWinter;
      
     case WindowMove:
      LogPseudoActions("Window move action to %.0f, %.0f processed.\n",
                                              action->data[0], action->data[1]);
      glfwSetWindowPos(window, (int)action->data[0], (int)action->data[1]);  
      LogWindowOperations("Window moved to screen coordinates %d, %d.\n",
                                        (int)action->data[0], (int)action->data[1]); 
      return WindowMove;

     case WindowResize:
      glfwSetWindowSize(window, (int)action->data[0], (int)action->data[1]);  
      LogWindowOperations("Window resized to width %d, height %d.\n",
                                        (int)action->data[0], (int)action->data[1]); 
      LogWindowOperations("Window resize action to width %d, height %d processed.\n",
                                      (int)action->data[0], (int)action->data[1]); 
      return WindowResize;

     default:
      LogRequestErrors("Unhandled action type in Window3D::processPseudoAction %d\n", 
                                                            action->actionType);
      return NoAction;
   }
}


// =======================================================================================
// Function used when we need to interact with the user prior to the main event loop
// starting.  We need to set up just enough context for imgGui to work.

int Window3D::initPanel(char* question, char** responses, int nResponses)
{
  while(!glfwWindowShouldClose(window))
   {
    glClearColor(0.6f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwGetWindowSize(window, &width, &height); // make sure we know current size
    Shader& shader = Shader::getMainShader();
    shader.useProgram();
    imgMenu->show_init_panel = true;
    int response = imgMenu->initPanel(question, responses, nResponses);
    glfwSwapBuffers(window);
    glfwPollEvents();    
    if(response >= 0)
      return response;
    if(checkGLError(stderr, "Window3D::initPanel"))
      exit(-1);
   }
  return -1;
}


// =======================================================================================
// Event processing loop for our window

void Window3D::loop(HttpLoadBalancer& httpServer)
{
  unsigned  frameCount = 0;
  Timeval   start;
  Timeval   frameTime;
  double    frameDouble;
  double    lastFrameDouble;
  bool      firstTime = true;

  start.now();

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
      LogFrameStarts("Frame %u starting at %.6lfs (%.1fms gap, year %.2f)\n",
                     frameCount, frameDouble, (frameDouble - lastFrameDouble)*1000.0f,
                     scene->getSimYear());
      if(frameTimeAvg == 0.0f)
        frameTimeAvg = frameDouble - lastFrameDouble;
      else
        frameTimeAvg = 0.001f*(frameDouble - lastFrameDouble) + 0.999f*frameTimeAvg;
     }
    
    // OpenGL calls to clear buffer
    glClearColor(0.6f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwGetWindowSize(window, &width, &height); // make sure we know current size
    
    scene->lock();

    // Do our actual drawing and deliver to screen window
    scene->draw(mouseMoved, (float)(frameDouble - lastFrameDouble));
    imgMenu->imguiInterface();
    glfwSwapBuffers(window);
    
    // Process pseudo-IO from HTTP interface
    InterfaceAction* action = NULL;
    while(int n = scene->actions.size())
     {
       action = scene->actions[n-1];
       if(processPseudoAction(action) == QuitProgram)
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
      processInput(scene->camera);
    else
      LogMouseLocation("ImGui has mouse\n");

    scene->unlock();

    // Final book-keeping for this spin of the loop
    lastFrameDouble = frameDouble;
    frameCount++;
   }
  
  // Shutdown stuff after main event loop is done.
  httpServer.shutDownNow = true;
  scene->saveState();
  frameTime.now();
  LogCloseDown("Orderly exit from window loop after %.6lf\n", frameTime - start);
  glfwTerminate();
}


// =======================================================================================
// We have detected a mouse click in the window - figure out what we should do.

void Window3D::processClick(float mouseX, float mouseY)
{
  LogMouseClick("Mouse click at %.2f, %.2f.\n", mouseX, mouseY);
  if(scene->focusObject)
   {
    unless(scene->focusObject == scene->lockObject)
     {
      // This is a new click-selection of an object
      LogMouseClick("Focussing on %s object at %.1f, %.1f, %.1f\n",
                  scene->focusObject->objectName(), scene->focusObjectLocation[0],
                  scene->focusObjectLocation[1], scene->focusObjectLocation[2]);
      scene->lockObject = scene->focusObject;
      glm_vec3_copy(scene->focusObjectLocation, scene->lockObjectLocation);
      scene->camera.setPivotLocation(scene->focusObjectLocation);
      imgMenu->show_lock_overlay = true;
     }
    else
     {
      // Clicking on a previouly selected object deselects it and means we have no
      // selection now.
      scene->lockObject = NULL;
      scene->camera.setPivotLocation(NULL);
      imgMenu->show_lock_overlay = false;
      LogMouseClick("User deselected %s object at %.1f, %.1f, %.1f\n",
                  scene->focusObject->objectName(), scene->focusObjectLocation[0],
                  scene->focusObjectLocation[1], scene->focusObjectLocation[2]);
     }
   }
}


// =======================================================================================
// We have detected a mouse double-click in the window - figure out what we should do.

void Window3D::processDoubleClick(float mouseX, float mouseY, float timeDiff)
{
  if(scene->simulationActive())
    scene->pauseSimulation();
  VisualObject* obj = scene->findObjectFromWindowCoords(scene->lastDoubleClick,
                          mouseX/width*2.0f-1.0f, 1.0f - mouseY/height*2.0f);
  const char* objName = obj->objectName();
  unless(obj)
   {
    LogDoubleClick("Double click (%.3fs) on nothing at %.2f, %.2f\n", 
                                                    timeDiff, mouseX, mouseY);
    return;
   }
  if(obj == scene->editModeObject)
   {
    // Double clicking again means we are deselecting it
    scene->processEditModeObjectDeselection();
   }
  if(strcmp("Bezier Patch", objName)==0 || strcmp("Land Surface Plane", objName)==0)
   {
    // Clicking on the land surface means we offer the option to insert something.
    imgMenu->show_insert_menu = true;
    LogDoubleClick("Double click insertion (%.3fs) at %.2f, %.2f\n", timeDiff, 
                                                                          mouseX, mouseY);
    return;
   }

  // Go into edit mode on the selected object.
  LogDoubleClick("Double click (%.3fs) to edit %s at %.2f, %.2f\n", timeDiff, 
                                                    objName, mouseX, mouseY);
  scene->editModeObject = obj;
  glm_vec3_copy(scene->lastDoubleClick, scene->editModeLocation);
  LogFlush();
  scene->processNewEditModeObject();
}


// =======================================================================================
// Handle mouse input in the window

void Window3D::processMouse(Camera& camera)
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
// Handle input in the window

// WASD is forward/back and lateral side-to-side.  Mouse movement is looking around.
// Minecraft has walk/fly mode, space jumps up into the air, but
// still follows ground contour.  E is a good button for interaction - convenient to
// WASD.*/

void Window3D::processInput(Camera& camera)
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
    float delta = timeDelta();
    if(scriptController)
      camOpFlags |= scriptController->simulatedKeys(delta);
    camera.adjust(camOpFlags, delta);
   }

  processMouse(camera);
}

// =======================================================================================
// Returns the number of microseconds since last call

float Window3D::timeDelta(void)
{
  Timeval thisMoment;
  thisMoment.now();
  double diff = (thisMoment - lastTime)*1e6;
  lastTime = thisMoment;
  
  return (float)diff;
}


// =======================================================================================
// Function called in the HTTP server thread to handle /window/ API calls.  This is a
// static method, because in future there might be more than one window.

bool Window3D::HTTPGateway(HttpDebug* serv, char* path)
{  
  if(strncmp(path, "resize/", 7)== 0) //XX this API will need rework for multiple windows
   {
    InterfaceAction* action = new InterfaceAction(WindowResize, path+7);
    if(action->valid)
     {
      theWin->scene->actions.push_back(action);
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
      theWin->scene->actions.push_back(action);
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
// Print instance debugging output

bool Window3D::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("Window3D: width: %d, height: %d<br>\n", width, height);
  
  return true;
}
