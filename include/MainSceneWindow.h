// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -

#ifndef MAIN_SCENE_WINDOW_H
#define MAIN_SCENE_WINDOW_H

#include "Window3D.h"


// =======================================================================================
/// @brief The main window for viewing the entire scene.
///
/// This subclass of Window3D is specialized to view the scene as a whole, and is the 
/// first window that comes up.  Other more specialized inspector windows can be spawned
/// from this.

class MainSceneWindow: public Window3D
{
public:
  
  // Instance variables - public

  // Member functions - public
  MainSceneWindow(int pixWidth, int pixHeight);
  ~MainSceneWindow(void);
  int   initPanel(char* question, char** responses, int nResponses);

protected:

  // The Window3D interface that we must implement.
  void        draw(float timeInterval);
  void        processClick(float mouseX, float mouseY);
  void        processDoubleClick(float mouseX, float mouseY, float timeDiff);
  bool        diagnosticHTMLRow(HttpDebug* serv, int rowIndex);
  bool        diagnosticHTML(HttpDebug* serv);
  ActionType  processAction(InterfaceAction* action);

private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  MainSceneWindow(const MainSceneWindow&);       
  /// @brief Prevent assignment.
  MainSceneWindow& operator=(const MainSceneWindow&);      

};

#endif




