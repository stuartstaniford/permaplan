// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Utility class for opening a window and setting up for drawing in using
// OpenGL commands.  The present implementation uses GLFW but the API should
// hide that.

#ifndef MENU_INTERFACE_H
#define MENU_INTERFACE_H
#include "DynamicallyTypable.h"
#include "ScriptController.h"
#include "Scene.h"
#include "imgui.h"
#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include <string>

//#define SHOW_DEMO_WINDOW

class Window3D;

class MenuInterface
{
 public:

  // Public instance variables
  Scene*          scene;
  bool            show_insert_menu;
  bool            show_lock_overlay;
  bool            show_init_panel;

  // Public methods
  MenuInterface(GLFWwindow* window, Window3D& W);
  ~MenuInterface(void);
  int   initPanel(char* question, char** responses, int nResponses);
  void  imguiInterface(void);
  bool  HTTPAPi(HttpDebug* serv, char* path);
  bool  HTTPAPiSelectGenus(HttpDebug* serv, char* path);
  bool  HTTPAPiAllTreeSelector(HttpDebug* serv, char* path);
  bool  HTTPAPiSelections(HttpDebug* serv, char* path);
  bool  HTTPAPiSimulate(HttpDebug* serv, char* path);
  bool  HTTPAPiInsert(HttpDebug* serv, char* path);
  bool  HTTPAPiEnter(HttpDebug* serv, char* path);
  bool  HTTPAPiOptions(HttpDebug* serv, char* path);
  void  allTreeSelectorPseudoAction(const char* optionName);
  void  allTreeSelectorButton(const char* name, DynamicallyTypable* value);
  void  insertHeightButton(void);
  void  insertBlockButton(void);
  void  insertTreeButton(void);
  void  heightEnteredButton(float z);
  void  blockEnteredButton(float blockSize, const std::string& matName);
  void  imguiTreeMenuButtonPressed(const char* genusString);

 private:
  
  // Class variables - private
  
  // Instance variables - private
  Window3D&           win3D;
  bool                show_height_input_dialog;
  bool                show_materials_menu;
  bool                show_tree_menu;
  const char*         genusSelected;
  bool                show_focus_overlay;
  bool                show_simulation_controller;
  bool                all_tree_selector;
  char                heightBuf[16];
  float               size;
#ifdef SHOW_DEMO_WINDOW
  bool                show_demo_window;
#endif

  // Private methods
  void  imguiInsertMenu(void);
  void  imguiHeightInputDialog(void);
  void  imguiMaterialsMenu(void);
  void  imguiGenusMenu(void);
  void  imguiTreeMenu(void);
  void  imguiAllTreeSelector(void);
  void  imguiFocusOverlay(void);
  void  imguiSimulationController(void);
  void  mouseOverlayDisplays(vec3 mouseSceneLoc);
  void  imguiLockOverlay(void);
  bool  createAction(HttpDebug* serv, ActionType actionType, 
                                            char* actionName, char* functionName, char* path);


  MenuInterface(const MenuInterface&);                 // Prevent copy-construction
  MenuInterface& operator=(const MenuInterface&);      // Prevent assignment

};

#endif
