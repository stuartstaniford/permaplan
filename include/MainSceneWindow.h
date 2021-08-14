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
  void draw(float timeInterval);
  int initPanel(char* question, char** responses, int nResponses);

private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  MainSceneWindow(const MainSceneWindow&);       
  /// @brief Prevent assignment.
  MainSceneWindow& operator=(const MainSceneWindow&);      

};

#endif




