// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
// This class is used for queueing up actions to be performed in subsequent frames
// (generally supplied by the HTTP Debug interface.

#include "InterfaceAction.h"
#include "Logging.h"

// =======================================================================================
// Constructor

InterfaceAction::InterfaceAction(ActionType type, char* path):
                                      actionType(type),
                                      valid(true)
{
  if(actionType == Click || actionType == DoubleClick)
   { 
    unless(extractColonVec2(path, mousePos))
    {
     LogRequestErrors("InterfaceAction::InterfaceAction couldn't get params from %s\n", path);
     valid = false;
    }
    return;
   }

  if(actionType == SimulateStart || actionType == SimulatePause)
    return;

  LogRequestErrors("InterfaceAction::InterfaceAction unknown action type.\n");
  valid = false;
}


// =======================================================================================
// Destructor

InterfaceAction::~InterfaceAction(void)
{
}


// =======================================================================================
