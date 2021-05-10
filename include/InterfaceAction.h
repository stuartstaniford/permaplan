// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
// This class is used for queueing up actions to be performed in subsequent frames
// (generally supplied by the HTTP Debug interface.

#ifndef INTERFACE_ACTION_H
#define INTERFACE_ACTION_H

#include "Global.h"


// =======================================================================================
// Enum for different types of action

enum ActionType
{
  Click,
  DoubleClick,
  SimulateStart,
  SimulatePause,
  SimulateReset,
  InsertHeight,
  InsertBlock,
  InsertTree,
  HeightEntered,
  QuitProgram,
};


// =======================================================================================
// Class variable initialization

class InterfaceAction
{
public:
  
  // Instance variables - public
  vec2 data;  
  ActionType actionType;
  bool valid;
  
  // Member functions - public
  InterfaceAction(ActionType type, char* path);
  ~InterfaceAction(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  InterfaceAction(const InterfaceAction&);                 // Prevent copy-construction
  InterfaceAction& operator=(const InterfaceAction&);      // Prevent assignment

};

#endif




