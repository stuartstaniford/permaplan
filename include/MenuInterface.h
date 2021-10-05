// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -

#ifndef MENU_INTERFACE_H
#define MENU_INTERFACE_H

#include "MenuPanel.h"


// =======================================================================================
// Forward declarations.

class Window3D;
class HttpDebug;


// =======================================================================================
/// @brief Superclass for classes that co-ordinate the overall menu interface for some
/// kind of window.
/// 
/// This class defines the interface for a master class in charge of the whole menu 
/// interface in some window - both the ImGui windows shown on screen as well as the 
/// HTTP interface to the various options that is used for scripting/testing.  It is 
/// where all the pointers live that point to instances of all the different individual 
/// menus and panels (which are all subclasses of MenuPanel.

class MenuInterface: public MenuPanel
{

public:

  // Public instance variables

  // Public methods
  MenuInterface(Window3D& W);
  ~MenuInterface(void);
  virtual void  imguiInterface(void);
  virtual bool  HTTPAPi(HttpDebug* serv, char* path);
  void  createErrorPanel(const char* errString);
  virtual ActionType processAction(InterfaceAction* action);

 protected:
  
  // Instance variables - protected
  Window3D&     win3D;

 private:
  
  // Instance variables - private
  
  // Private methods
  /// @brief Prevent copy-construction
  MenuInterface(const MenuInterface&);       
  /// @brief Prevent assignment
  MenuInterface& operator=(const MenuInterface&);     

};


// =======================================================================================

#endif
