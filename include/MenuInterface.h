// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Utility class for opening a window and setting up for drawing in using
// OpenGL commands.  The present implementation uses GLFW but the API should
// hide that.

#ifndef MENU_INTERFACE_H
#define MENU_INTERFACE_H
#include "imgui.h"
#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ScriptController.h"
#include "Scene.h"

//#define SHOW_DEMO_WINDOW

class Window3D;

class MenuInterface
{
 public:

  // Public instance variables
  Scene*          scene;
  bool            show_insert_menu;

  // Public methods
  MenuInterface(GLFWwindow* window, Window3D& W);
  ~MenuInterface(void);
  void  imguiInterface(void);
  void  imguiInsertMenu(void);
  void  imguiHeightInputDialog(void);
  void  imguiMaterialsMenu(void);
  void  imguiGenusMenu(void);
  void  imguiTreeMenu(void);
  void  imguiFocusOverlay(void);
  void  mouseOverlayDisplays(vec3 mouseSceneLoc);

 private:
  
  // Class variables - private
  
  // Instance variables - private
  Window3D&       win3D;
  bool            show_height_input_dialog;
  bool            show_materials_menu;
  bool            show_tree_menu;
  const char*     genusSelected;
  bool            show_focus_overlay;
#ifdef SHOW_DEMO_WINDOW
  bool            show_demo_window;
#endif
  char            heightBuf[16];
  float           size;

  // Private methods
  
  MenuInterface(const MenuInterface&);                 // Prevent copy-construction
  MenuInterface& operator=(const MenuInterface&);      // Prevent assignment

};

#endif
