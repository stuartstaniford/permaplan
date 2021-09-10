// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -

#ifndef MENU_INTERFACE_H
#define MENU_INTERFACE_H
#include "DynamicallyTypable.h"
#include "MenuPanel.h"
#include <cglm/cglm.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include <string>


// =======================================================================================
// Turn this to show the Dear Imgui demo window

//#define SHOW_DEMO_WINDOW


// =======================================================================================
// Forward declarations.

class Window3D;
class MainSceneWindow;
class MenuBlockPanel;
class MenuInsert;
class MenuTree;
class MenuGenus;
class MenuGablePanel;
class MenuHeightPanel;
class MenuShedPanel;
class MenuAllTree;


// =======================================================================================
/// @brief Co-ordinate the overall menu interface.
/// 
/// This class is the master class in charge of the whole menu interface - both the 
/// ImGui windows shown on screen as well as the HTTP interface to the various options
/// that is used for scripting/testing.  It is where all the pointers live that point
/// to instances of all the different individuals menus and panels (which are all
/// subclasses of MenuPanel.

class MenuInterface: public MenuPanel
{
  friend Window3D;
  friend MainSceneWindow;
  friend MenuBlockPanel;
  friend MenuInsert;
  friend MenuTree;
  friend MenuGenus;
  friend MenuGablePanel;
  friend MenuShedPanel;
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
  ActionType processAction(InterfaceAction* action);

 private:
  
  // Class variables - private
  
  // Instance variables - private
  Window3D&     win3D;
  MenuPanel*    shedPanel;
  MenuPanel*    gablePanel;
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
