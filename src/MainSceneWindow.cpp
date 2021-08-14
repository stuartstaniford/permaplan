// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -
// This subclass of Window3D is specialized to view the scene as a whole, and is the 
// first window that comes up.  Other more specialized inspector windows can be spawned
// from this.

#include "MainSceneWindow.h"


// =======================================================================================
/// @brief Constructor

MainSceneWindow::MainSceneWindow(int pixWidth, int pixHeight):
                            Window3D(pixWidth, pixHeight)
{
}


// =======================================================================================
/// @brief Destructor

MainSceneWindow::~MainSceneWindow(void)
{
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
