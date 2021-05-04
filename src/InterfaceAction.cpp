// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
// This class is used for queueing up actions to be performed in subsequent frames
// (generally supplied by the HTTP Debug interface.

#include "InterfaceAction.h"

// =======================================================================================
// Constructor

InterfaceAction::InterfaceAction(ActionType type, char* path):
                                      actionType(type),
                                      valid(true)
{
  unless(extractColonVec2(path, mousePos))
    valid = false;
}


// =======================================================================================
// Destructor

InterfaceAction::~InterfaceAction(void)
{
}


// =======================================================================================
