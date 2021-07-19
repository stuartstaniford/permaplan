// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
// This class is used for queueing up actions to be performed in subsequent frames
// (generally supplied by the HTTP Debug interface.

#include "InterfaceAction.h"
#include "Species.h"
#include "Logging.h"


// =======================================================================================
// Constructor

InterfaceAction::InterfaceAction(ActionType type, char* path):
                                      actionType(type),
                                      otherData(NULL),
                                      valid(true)
{
  // Clicks, double-clicks, etc
  if(actionType == Click || actionType == DoubleClick)
   { 
    unless(extractColonVec2(path, data))
    {
     LogRequestErrors("InterfaceAction::InterfaceAction couldn't get click "
                                                                "params from %s\n", path);
     valid = false;
    }
    return;
   }

  if(actionType == CameraMoved)
    return;

  // Simulation controls
  if(actionType == SimulateStart || actionType == SimulatePause 
     || actionType == SimulateReset  || actionType == SimulateSpring
     || actionType == SimulateSummer || actionType == SimulateFall
     || actionType == SimulateWinter)
    return;

  // Insert menu options
  if(actionType == InsertHeight || actionType == InsertBlock || actionType == InsertShed ||
     actionType == InsertGable || actionType == InsertTree)
    return;
  
  // Result of height entry panel
  if(actionType == HeightEntered)
   {
    unless(extractColonFloat(path, data))
    {
     LogRequestErrors("InterfaceAction::InterfaceAction couldn't get HeightEntered "
                                                                "params from %s\n", path);
     valid = false;
    }
    return;    
   }

  // Result of block entry panel
  if(actionType == BlockEntered)
   {
    unless(extractColonFloat(path, data))
    {
     LogRequestErrors("InterfaceAction::InterfaceAction couldn't get BlockEntered "
                                                                "params from %s\n", path);
     valid = false;
    }
    return;    
   }

  if(actionType == GableEntered || actionType == ShedEntered)
    return; 
  
  // Tree select genus menu.  Note path was already checked in 
  // MenuInterface::HTTPAPiSelectGenus
  if(actionType == SelectGenus)
   {
    otherData = new SpeciesPath(path);  
    return;
   }

  // All tree selector.  Note path has *not* been sanity checked yet except for crazy length
  if(actionType == AllTreeSelection)
   {
    otherData = new SpeciesPath(path);  
    return;
   }
  
  // Window resize
  if(actionType == WindowMove || actionType == WindowResize)
   { 
    unless(extractColonVec2(path, data))
    {
     LogRequestErrors("InterfaceAction::InterfaceAction couldn't get window "
                                                                "params from %s\n", path);
     valid = false;
    }
    return;
   }

  // QuitProgram
  if(actionType == QuitProgram)
    return;
  
  LogRequestErrors("InterfaceAction::InterfaceAction unknown action type.\n");
  valid = false;
}


// =======================================================================================
// Destructor

InterfaceAction::~InterfaceAction(void)
{
  if(otherData)
    delete otherData;
}


// =======================================================================================
