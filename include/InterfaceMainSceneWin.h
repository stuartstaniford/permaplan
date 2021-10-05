// Copyright Staniford Systems.  All Rights Reserved.  October 2021 -

#ifndef INTERFACE_MAIN_SCENE_WIN_H
#define INTERFACE_MAIN_SCENE_WIN_H

#include "MenuInterface.h"


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
/// @brief Coordinate the overall menu interface for the MainSceneWindow
///
/// This implements the interface defined in MenuInterface.h for the specific case of the
/// MainSceneWindow (the one that opens when the application opens with an overview
/// of the overall scene/design.

class InterfaceMainSceneWin: public MenuInterface
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
  
  // Instance variables - public
  bool            show_lock_overlay;
  
  // Member functions - public
  InterfaceMainSceneWin(Window3D& W);
  ~InterfaceMainSceneWin(void);
  void  imguiInterface(void);
  bool  HTTPAPi(HttpDebug* serv, char* path);
  bool  HTTPAPiSelections(HttpDebug* serv, char* path);
  bool  HTTPAPiEnter(HttpDebug* serv, char* path);
  bool  HTTPAPiOptions(HttpDebug* serv, char* path);
  void  createErrorPanel(const char* errString);
  ActionType processAction(InterfaceAction* action);
  
private:
  
  // Instance variables - private
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
  bool          show_demo_window;
#endif

  // Member functions - private
  void  imguiLockOverlay(void);
  /// @brief Prevent copy-construction.
  InterfaceMainSceneWin(const InterfaceMainSceneWin&);       
  /// @brief Prevent assignment.
  InterfaceMainSceneWin& operator=(const InterfaceMainSceneWin&);      
};


// =======================================================================================

#endif




