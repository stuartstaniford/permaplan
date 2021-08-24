// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -
// This subclass of Window3D is specialized to view the scene as a whole, and is the 
// first window that comes up.  Other more specialized inspector windows can be spawned
// from this.

#include "MainSceneWindow.h"
#include "MenuInterface.h"
#include "MenuInsert.h"
#include "TreeWindow.h"

// =======================================================================================
/// @brief Constructor

MainSceneWindow::MainSceneWindow(int pixWidth, int pixHeight):
                                  Window3D(pixWidth, pixHeight, 
                                           (char*)"Landscape Simulation")
{
  // Dear ImGui initialization
  imgMenu = new MenuInterface(*this);
}


// =======================================================================================
/// @brief Destructor

MainSceneWindow::~MainSceneWindow(void)
{
}


// =======================================================================================
/// @brief Render the scene into our window

void MainSceneWindow::draw(float timeInterval)
{
  scene->draw(camera, timeInterval);
  imgMenu->imguiInterface();
}


// =======================================================================================
/// @brief We have been notified of a mouse click in the window - figure out what we 
/// should do.
/// @param mouseX X location of mouse click in window co-ordinates
/// @param mouseY Y location of mouse click in window co-ordinates

void MainSceneWindow::processClick(float mouseX, float mouseY)
{
  LogMouseClick("Mouse click in %s at %.2f, %.2f.\n", winTitle, mouseX, mouseY);
  if(scene->focusObject)
   {
    unless(scene->focusObject == scene->lockObject)
     {
      // This is a new click-selection of an object
      LogMouseClick("Focussing on %s object in %s at %.1f, %.1f, %.1f\n",
                  scene->focusObject->objectName(), winTitle, 
                  scene->focusObjectLocation[0], scene->focusObjectLocation[1], 
                  scene->focusObjectLocation[2]);
      scene->lockObject = scene->focusObject;
      glm_vec3_copy(scene->focusObjectLocation, scene->lockObjectLocation);
      camera.setPivotLocation(scene->focusObjectLocation);
      imgMenu->show_lock_overlay = true;
     }
    else
     {
      // Clicking on a previouly selected object deselects it and means we have no
      // selection now.
      scene->lockObject = NULL;
      camera.setPivotLocation(NULL);
      imgMenu->show_lock_overlay = false;
      LogMouseClick("User deselected %s object in %s at %.1f, %.1f, %.1f\n",
                  scene->focusObject->objectName(), winTitle,
                  scene->focusObjectLocation[0], scene->focusObjectLocation[1], 
                  scene->focusObjectLocation[2]);
     }
   }
}


// =======================================================================================
/// @brief We have detected a mouse double-click in the window, now figure out what we 
/// should do.
///
/// @param mouseX X location of mouse click in window co-ordinates
/// @param mouseY Y location of mouse click in window co-ordinates
/// @param timeDiff the interval between the two clicks in seconds
/// @todo double-click should not result in an insert menu if any floating menus are
/// already on screen.

void MainSceneWindow::processDoubleClick(float mouseX, float mouseY, float timeDiff)
{
  if(scene->simulationActive())
    scene->pauseSimulation();
  VisualObject* obj = scene->findObjectFromWindowCoords(camera, scene->lastDoubleClick,
                          mouseX/width*2.0f-1.0f, 1.0f - mouseY/height*2.0f);
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
    return;
   }
  DynamicType objType = obj->getDynamicType();
  if(objType == TypeBezierPatch || objType == TypeLandSurfaceRegionPlanar)
   {
    // Clicking on the land surface means we offer the option to insert something.
    if(!imgMenu->insertMenu)
     {
      imgMenu->insertMenu = new MenuInsert(imgMenu, scene);
      LogDoubleClick("Double click insertion (%.3fs) at %.2f, %.2f\n", timeDiff, 
                                                                          mouseX, mouseY);
     }
    return;
   }
  if(objType == TypeTree)
   {
    TreeWindow* treeWin = new TreeWindow;
    treeWin->loop();
    return;
   }

  // Go into edit mode on the selected object.
  LogDoubleClick("Double click (%.3fs) to edit %s at %.2f, %.2f\n", timeDiff, 
                                                        obj->objectName(), mouseX, mouseY);
  scene->editModeObject = obj;
  glm_vec3_copy(scene->lastDoubleClick, scene->editModeLocation);
  LogFlush();
  scene->processNewEditModeObject();
}


// =======================================================================================
/// @brief Function used when we need to interact with the user prior to the main event 
/// loop starting.
///   
/// We need to set up just enough context for imgGui to work.  The model is that the 
/// caller supplies us with a question and a series of responses which are placed on
/// buttons, and the user picks a button and we return their choice to the caller.
/// @returns An integer indicating which of the nResponses options was chosen.
/// @param question A C-string with the question on which we need the user's input.
/// @param response A pointer to an array of C-strings with the possible respones to the
/// question.
/// @param nResponses The number of responses (which are taken to run 0..nRespones-1).

int MainSceneWindow::initPanel(char* question, char** responses, int nResponses)
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
    if(checkGLError(stderr, "MainSceneWindow::initPanel"))
      exit(-1);
   }
  return -1;
}


// =======================================================================================
/// @brief Handle an pseudo-interface event coming from the HTTP Debug interface.  
/// 
/// We first check if our parent Window3D can handle it, and if not we pass it to our
/// menu system for processing..
/// @returns The ActionType of the action we processed.
/// @param action The InterfaceAction that needs to be handled.

ActionType MainSceneWindow::processAction(InterfaceAction* action)
{  
  ActionType parentAction = Window3D::processAction(action);
  unless(parentAction == NoAction)
    return parentAction;
  
  switch(action->actionType)
   {
    default:
      return imgMenu->processAction(action);
   }
}


// =======================================================================================
/// @brief Function called in the HTTP server thread to provide one row of a table about
/// our particular window instance.  
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool MainSceneWindow::diagnosticHTMLRow(HttpDebug* serv, int rowIndex)
{
  httPrintf("<tr><td>%d</td>"
            "<td><a href=\"/window/detail/%d/\">%s</a></td>"
            "<td>Details</td></tr>\n", rowIndex, ourWin, winTitle);
  return true;
}


// =======================================================================================
/// @brief Function called in the HTTP server thread to give details of our window.  
/// @returns True if the page was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool MainSceneWindow::diagnosticHTML(HttpDebug* serv)
{
  unless(serv->startResponsePage("Main Scene Window"))
    return false;

  unless(serv->endResponsePage())
    return false;
  return true;
}


// =======================================================================================
