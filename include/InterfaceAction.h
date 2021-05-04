// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
// This class is used for queueing up actions to be performed in subsequent frames
// (generally supplied by the HTTP Debug interface.

#ifndef INTERFACE_ACTION_H
#define INTERFACE_ACTION_H


// =======================================================================================
// Enum for different types of action

enum ActionType
{
  Click,
  DoubleClick,
};


// =======================================================================================
// Class variable initialization

class InterfaceAction
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  InterfaceAction(ActionType type, char* path);
  ~InterfaceAction(void);
  
private:
  
  // Instance variables - private
  ActionType actionType;
  unsigned mouseX;
  unsigned mouseY;
  
  // Member functions - private
  InterfaceAction(const InterfaceAction&);                 // Prevent copy-construction
  InterfaceAction& operator=(const InterfaceAction&);      // Prevent assignment

};

#endif




