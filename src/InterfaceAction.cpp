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
  // Clicks, double-clicks, etc
  if(actionType == Click || actionType == DoubleClick)
   { 
    unless(extractColonVec2(path, data))
    {
     LogRequestErrors("InterfaceAction::InterfaceAction couldn't get params from %s\n", path);
     valid = false;
    }
    return;
   }

  // Simulation controls
  if(actionType == SimulateStart || actionType == SimulatePause || actionType == SimulateReset)
    return;

  // Insert menu options
  if(actionType == InsertHeight || actionType == InsertBlock || actionType == InsertTree)
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
