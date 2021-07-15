// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -

#ifndef MENU_INTERFACE_H
#define MENU_INTERFACE_H
#include "DynamicallyTypable.h"
#include "MenuPanel.h"
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
class MenuBlockPanel;
class MenuInsert;
class MenuTree;
class MenuGenus;
class MenuHeightPanel;
class MenuAllTree;

class MenuInterface: public MenuPanel
{
  friend Window3D;
  friend MenuBlockPanel;
  friend MenuInsert;
  friend MenuTree;
  friend MenuGenus;
  friend MenuHeightPanel;
  friend MenuAllTree;

public:

  // Public instance variables
  bool            show_lock_overlay;
  bool            show_init_panel;

  // Public methods
  MenuInterface(Window3D& W);
  ~MenuInterface(void);
  int   initPanel(char* question, char** responses, int nResponses);
  void  imguiInterface(void);
  bool  HTTPAPi(HttpDebug* serv, char* path);
  bool  HTTPAPiSelections(HttpDebug* serv, char* path);
  bool  HTTPAPiEnter(HttpDebug* serv, char* path);
  bool  HTTPAPiOptions(HttpDebug* serv, char* path);
  void  createErrorPanel(const char* errString);

 private:
  
  // Class variables - private
  
  // Instance variables - private
  Window3D&     win3D;
  MenuPanel*    shedPanel;
  MenuPanel*    blockPanel;
  MenuPanel*    heightPanel;
  MenuPanel*    simulationPanel;
  MenuPanel*    focusOverlay;
  MenuPanel*    insertMenu;
  MenuPanel*    treeMenu;
  MenuPanel*    genusMenu;
  MenuPanel*    allTreeMenu;

#ifdef SHOW_DEMO_WINDOW
  bool                show_demo_window;
#endif

  // Private methods
  void  imguiLockOverlay(void);
  /// @brief Prevent copy-construction
  MenuInterface(const MenuInterface&);       
  /// @brief Prevent assignment
  MenuInterface& operator=(const MenuInterface&);     

};

#endif
