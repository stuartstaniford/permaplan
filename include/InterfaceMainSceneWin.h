// Copyright Staniford Systems.  All Rights Reserved.  October 2021 -

#ifndef INTERFACE_MAIN_SCENE_WIN_H
#define INTERFACE_MAIN_SCENE_WIN_H


// =======================================================================================
/// @brief Coordinate the overall menu interface for the MainSceneWindow
///
/// This implements the interface defined in MenuInterface.h for the specific case of the
/// MainSceneWindow (the one that opens when the application opens with an overview
/// of the overall scene/design.

class InterfaceMainSceneWin
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  InterfaceMainSceneWin(void);
  ~InterfaceMainSceneWin(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  InterfaceMainSceneWin(const InterfaceMainSceneWin&);       
  /// @brief Prevent assignment.
  InterfaceMainSceneWin& operator=(const InterfaceMainSceneWin&);      
};


// =======================================================================================

#endif




