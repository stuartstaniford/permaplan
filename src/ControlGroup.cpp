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
  LogControlGroupInit("Started new control group around object of type %s.\n",
                        firstObject->objectName());
  
  // Add the horizonal movement arrows
/*  float arrowLength = masterObject->box->height()*0.5f;
  float arrowSpacing = arrowLength*0.4f;
  vec3 pos, dir;
  vec3 centroid;
  masterObject->box->getCentroid(centroid);
  pos[2] = 
  */
  // North arrow
  
  
}


// =======================================================================================
// Destructor

ControlGroup::~ControlGroup(void)
{
}


// =======================================================================================
// Tell callers our name at runtime.

const char* ControlGroup::objectName(void)
{
  static char* name = (char*)"Control Group";
  return name;
}


// =======================================================================================
