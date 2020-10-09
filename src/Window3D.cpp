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
#include "Window3D.h"
#include "PmodException.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


// =======================================================================================
// For overall openGL initialization

bool GLFWInitDone = false;


// =======================================================================================
// Calback for window resizing

void windowResize(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}


// =======================================================================================
// Constructor for a Window3D

Window3D::Window3D(int pixWidth, int pixHeight):
                        scene(NULL),
                        scriptController(NULL),
                        width(pixWidth),
                        height(pixHeight),
                        lastMouseX(HUGE_VAL),
                        lastMouseY(HUGE_VAL),
                        show_insert_menu(false),
                        show_focus_overlay(true),
                        inClick(false),
                        testingDoubleClick(false),
                        mouseMoved(true),
                        frameTimeAvg(0.0f)
#ifdef SHOW_DEMO_WINDOW
                        , show_demo_window(true)
#endif
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
 
  // Dear ImGui initialization
  // Setup context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 410");
  // Setup Dear ImGui style
  ImGui::StyleColorsClassic();
  
  lastTime.now();
}


// =======================================================================================
// The floating menu to insert stuff at the focus point

void Window3D::imguiInsertMenu(void)
{
  if(!show_insert_menu)
    return;
  ImGui::Begin("Insert", &show_insert_menu, ImGuiWindowFlags_AlwaysAutoResize);
  
  ImGui::InputText("", heightBuf, 8, ImGuiInputTextFlags_CharsDecimal);
  
  bool fromScript;
  if(ImGui::Button("Height") ||
          (fromScript = scriptController->checkInterfaceAction(IA_HeightMarker)))
   {
    float z;
    if(fromScript)
      z = scriptController->getHeight();
    else
      z = atof(heightBuf);
    
    heightBuf[0] = '\0';
    scene->lastDoubleClick[2] = z;
    scene->newLandHeight(scene->lastDoubleClick);
    show_insert_menu = false;
   }
  ImGui::End();
}


// =======================================================================================
// Utility function to put a matrix into the current imgui window.

void displayImguiMatrix(const char* title, const mat4& m)
{
  ImGui::Text("%s\n", title);
  for(int i=0; i<4; i++)
    ImGui::Text("%.2f %.2f %.2f %.2f\n", m[i][0], m[i][1], m[i][2], m[i][3]);
}


// =======================================================================================
// Stuff that may or may not be turned on in the overlay window concerning the mouse
// location.

void Window3D::mouseOverlayDisplays(vec3 mouseSceneLoc)
{
  //ImGui::Text("Mouse: %.1f E, %.1f N %.1f altitude\n",
  //            mouseSceneLoc[0], mouseSceneLoc[1], mouseSceneLoc[2]);
  
  // Shows location of mouse in window co-ordinates
  /*ImGui::Text("Raw mouse: %.1f/%d, %.1f/%d\n", lastMouseX, width, lastMouseY, height);
  ImGui::Text("Clip mouse: %.3f [-1,1], %.3f [-1,1]\n",
              lastMouseX/width*2.0f-1.0f, 1.0f - lastMouseY/height*2.0f);
  */
  //Mouse Ray
  /*ImGui::Text("Mouse in world space: %.3f, %.3f, %.3f'\n", scene->lastMouseLocation[0],
              scene->lastMouseLocation[1], scene->lastMouseLocation[2]);
  ImGui::Text("Mouse direction: %.3f, %.3f, %.3f'\n", scene->lastMouseDirection[0],
              scene->lastMouseDirection[1], scene->lastMouseDirection[2]);
  */
  // Projection matrix
  //displayImguiMatrix("Projection Matrix:", scene->camera.projection);
  
  // View matrix
  //displayImguiMatrix("View Matrix:", scene->camera.view);
  
  // Model matrix
  //displayImguiMatrix("Model Matrix:", scene->model);
  
  // Invert matrix
  //displayImguiMatrix("Invert Matrix:", scene->invert);
}

// =======================================================================================
// Transparent overlay that displays information about the thing currently in focus

void Window3D::imguiFocusOverlay(void)
{
  if(!show_focus_overlay)
    return;
  const float DISTANCE = 10.0f;
  static int corner = 0;
  ImGuiIO& io = ImGui::GetIO();
  
  if (corner != -1)
   {
    ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE :
                               DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
   }
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
  if (corner != -1)
    window_flags |= ImGuiWindowFlags_NoMove;
  if (ImGui::Begin("Focus Overlay", &show_focus_overlay, window_flags))
   {
    vec3 mouseSceneLoc;
    //scene->findCameraObject(camF);
    VisualObject* mouseObject = scene->findObjectFromWindowCoords(mouseSceneLoc,
                    lastMouseX/width*2.0f-1.0f, 1.0f - lastMouseY/height*2.0f);
    if(mouseObject)
     {
      ImGui::Text("Object Type: %s\n", mouseObject->objectName());
      ImGui::Text("Coords: %.1f' east, %.1f' north\nAltitude: %.1f'\n",
                  mouseSceneLoc[0], mouseSceneLoc[1], mouseSceneLoc[2]);
      mouseOverlayDisplays(mouseSceneLoc);
     }
    else
      ImGui::Text("Mouse out of scene.\n");

    ImGui::Text("Camera Height: %.1f'\n", scene->findCameraHeight());
    ImGui::Text("Frames/Sec: %.0f\n", 1.0f/frameTimeAvg);
    ImGui::Separator();
   }
  ImGui::End();
}


// =======================================================================================
// Top level function to organize the rendering of all the imguiInterface stuff

void Window3D::imguiInterface(void)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  
  imguiInsertMenu();
  
  imguiFocusOverlay();

#ifdef SHOW_DEMO_WINDOW
  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);
#endif
  
  // Render dear imgui into screen
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


// =======================================================================================
// Event processing loop for our window

void Window3D::loop(void)
{
  unsigned  frameCount = 0;
  Timeval   start;
  Timeval   frameTime;
  double    frameDouble;
  double    lastFrameDouble;
  bool      firstTime = true;

  start.now();

  while(!glfwWindowShouldClose(window))
   {
    frameTime.now();
    frameDouble = frameTime - start;
    if(firstTime)
     {
      LogFrameStarts("Frame %u starting at %.6lfs\n", frameCount, frameDouble);
      firstTime = false;
     }
    else
     {
      LogFrameStarts("Frame %u starting at %.6lfs (%.1fms gap)\n",
                     frameCount, frameDouble, (frameDouble - lastFrameDouble)*1000.0f);
      if(frameTimeAvg == 0.0f)
        frameTimeAvg = frameDouble - lastFrameDouble;
      else
        frameTimeAvg = 0.001f*(frameDouble - lastFrameDouble) + 0.999f*frameTimeAvg;
     }
    lastFrameDouble = frameDouble;
    glClearColor(0.6f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwGetWindowSize(window, &width, &height); // make sure we know current size
    
    scene->draw(mouseMoved);
    imguiInterface();
    ImGuiIO& io = ImGui::GetIO();

    glfwSwapBuffers(window);
    glfwPollEvents();
    unless(io.WantCaptureMouse)
      processInput(scene->camera);
    else
      LogMouseLocation("ImGui has mouse\n");

    frameCount++;
   }
}

// =======================================================================================
// We have detected a mouse double-click in the window - figure out what we should do.

void Window3D::processDoubleClick(float mouseX, float mouseY, float timeDiff)
{
  show_insert_menu = true;
  scene->findObjectFromWindowCoords(scene->lastDoubleClick,
                          mouseX/width*2.0f-1.0f, 1.0f - mouseY/height*2.0f);
  LogDoubleClick("Double click (%.3fs) at %.2f, %.2f\n", timeDiff, mouseX, mouseY);
}


// =======================================================================================
// Handle mouse input in the window

void Window3D::processMouse(Camera& camera)
{
  if (!glfwGetWindowAttrib(window, GLFW_HOVERED))
   {
    LogMouseLocation("Mouse cursor out of window\n");
    return; // cursor is not with us, nothing to do.
   }
  
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);

  if( (abs(mouseX - lastMouseX) < epsilon) && (abs(mouseY - lastMouseY) < epsilon))
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
  if (state == GLFW_PRESS)
   {
    if(!inClick && !testingDoubleClick)
      LogMouseClick("Mouse click started.\n");
    inClick = true;
    if(testingDoubleClick)
     {
      testingDoubleClick = false;
      Timeval justNow;
      justNow.now();
      float timeDiff = justNow - mouseUpTime;
      if(timeDiff < 0.25)
        processDoubleClick((float)mouseX, (float)mouseY, timeDiff);
     }
    else if(mouseMoved)
     {
      double xDelta = (mouseX - lastMouseX)/width;
      double yDelta = (mouseY - lastMouseY)/height;
      if(xDelta < -1.0 || xDelta > 1.0 || yDelta < -1.0 || yDelta > 1.0)
        goto processMouseExit; // weird, don't know what to do.
      camera.mouseDrag((float)xDelta, (float)yDelta);
     }
   }
  else if(inClick)
   {
    inClick            = false;
    testingDoubleClick = true;
    mouseUpTime.now();
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
// Print instance debugging output

void Window3D::diagnosticHTML(void)
{
  printf("Window3D: width: %d, height: %d<br>\n", width, height);
}
