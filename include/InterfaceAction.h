// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
// This class is used for queueing up actions to be performed in subsequent frames
// (generally supplied by the HTTP Debug interface.

#ifndef INTERFACE_ACTION_H
#define INTERFACE_ACTION_H

#include "Global.h"
#include "DynamicallyTypable.h"


// =======================================================================================
// Enum for different types of action

enum ActionType
{
  NoAction,
  AllTreeSelection,
  BlockEntered,
  CameraMoved,
  Click,
  DoubleClick,
  HeightEntered,
  InsertHeight,
  InsertBlock,
  InsertTree,
  QuitProgram,
  SelectGenus,
  SimulatePause,
  SimulateReset,
  SimulateStart,
  WindowMove,
  WindowResize,
};


// =======================================================================================
// Class variable initialization

class InterfaceAction
{
public:
  
  // Instance variables - public
  ActionType actionType;
  vec2 data;  
  DynamicallyTypable* otherData; //points to a copy we own & will delete in ~InterfaceAction
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




