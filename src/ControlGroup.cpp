// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is a subclass of ObjectGroup specifically to handle the visual controls
// that appear around an object after it has been double-clicked.

#include "ControlGroup.h"

// =======================================================================================
// Constructor

ControlGroup::ControlGroup(VisualObject* firstObject):
                                  ObjectGroup(firstObject)
{
  masterObject = firstObject; // the main object - everything else is ephemeral controls
}


// =======================================================================================
// Destructor

ControlGroup::~ControlGroup(void)
{
}


// =======================================================================================
