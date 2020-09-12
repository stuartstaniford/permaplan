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
#include "Window3D.h"
#include "PmodException.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"


// =======================================================================================
// Class variable initialization

bool Window3D::GLFWInitDone = false;


// =======================================================================================
// Calback for window resizing

void windowResize(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}


// =======================================================================================
// Constructor for a Window3D

Window3D::Window3D(int pixWidth, int pixHeight): scene(NULL),
                    scriptController(NULL), width(pixWidth),
                    height(pixHeight), lastMouseX(HUGE_VAL), lastMouseY(HUGE_VAL),
                    show_insert_menu(false), show_focus_overlay(true), inClick(false),
                    testingDoubleClick(false)
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
    scene->newLandHeight(z);
    show_insert_menu = false;
   }
  ImGui::End();
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
    vec3 camF;
    scene->findCameraObject(camF);
    ImGui::Text("Object Type\nCoords: %.1f' east, %.1f' north\nAltitude: %.1f'\n",
                  camF[0], camF[1], camF[2]);
    ImGui::Text("Camera Height: %.1f'\n", scene->findCameraHeight());
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
  while(!glfwWindowShouldClose(window))
   {
    glClearColor(0.6f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    scene->draw();
    imguiInterface();
    ImGuiIO& io = ImGui::GetIO();

    glfwSwapBuffers(window);
    glfwPollEvents();
    if(!io.WantCaptureMouse)
      processInput(scene->camera);
   }
}

// =======================================================================================
// We have detected a mouse double-click in the window - figure out what we should do.

void Window3D::processDoubleClick(float mouseX, float mouseY)
{
  show_insert_menu = true;
  printf("Double click at %.2f, %.2f!!\n", mouseX, mouseY);
}


// =======================================================================================
// Handle mouse input in the window

void Window3D::processMouse(Camera& camera)
{
  if (!glfwGetWindowAttrib(window, GLFW_HOVERED))
    return; // cursor is not with us, nothing to do.
  
  double mouseX, mouseY;
  glfwGetCursorPos(window, &mouseX, &mouseY);
  
  int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
  if (state == GLFW_PRESS)
   {
    inClick = true;
    if(testingDoubleClick)
     {
      testingDoubleClick = false;
      Timeval justNow;
      justNow.now();
      if(justNow - mouseUpTime < 0.25)
        processDoubleClick((float)mouseX, (float)mouseY);
     }
    else
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
