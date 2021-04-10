// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is for a group of objects that are all to be displayed and manipulated as
// a unit (and appear in the Quadtree together).


#include "ObjectGroup.h"


// =======================================================================================
// Constructor

ObjectGroup::ObjectGroup(VisualObject* firstObject):
                        VisualObject(false)
{
  insert(firstObject);
  firstObject->groupOwner = this;
  updateBoundingBox();
  LogGroupAdditions("Creating new object group with object of type %s.\n", 
                                                                firstObject->objectName());
}


// =======================================================================================
// Destructor

ObjectGroup::~ObjectGroup(void)
{
}


// =======================================================================================
// Add an additional object into this group

void ObjectGroup::add(VisualObject* nextObject)
{
  insert(nextObject);
  nextObject->groupOwner = this;
  vec3 offset = {0.0f, 0.0f, 0.0f};
  bool change = nextObject->updateBoundingBox(box, offset);
  if(change)
   {
    LogGroupAdditions("Add to group, object of type %s (changed bbox).\n", 
                                                              nextObject->objectName());
   }
  else
   {
    LogGroupAdditions("Add to group, object of type %s (no change in bbox).\n", 
                                                              nextObject->objectName());
   }
}


// =======================================================================================
// Compute the bounding box that is the union of all our member bounding boxes

bool ObjectGroup::updateBoundingBox(BoundingBox* B, vec3 offset)
{
  bool retVal = false;
  
  for(VisualObject* V: *this)
    if(V->updateBoundingBox(B, offset))
      retVal = true;
  
  return retVal;
} 


// =======================================================================================
// Recurse into all our parts and update our bounding box

void ObjectGroup::updateBoundingBox(void)
{
  vec3 offset = {0.0f, 0.0f, 0.0f};
  
  if(!box)
    box = new BoundingBox();
  for(VisualObject* V: *this)
    V->updateBoundingBox(box, offset);
}


// =======================================================================================
// Decide if a ray touches us.  Check all the kids and return the closest.

bool ObjectGroup::matchRay(vec3& position, vec3& direction, float& lambda)
{
  unless(box->matchRay(position, direction, lambda))
    return false;
  
  float bestLambda = HUGE_VALF;
  float thisLambda;
  bool retVal = false;
  for(VisualObject* V: *this)
   {
    if(V->matchRay(position, direction, thisLambda))
     {
      retVal = true;
      if(thisLambda < bestLambda)
        bestLambda = thisLambda;
     }
   }
  return retVal;
}


// =======================================================================================
// Tell callers our name at runtime.

const char* ObjectGroup::objectName(void)
{
  static char* name = (char*)"Object Group";
  return name;
}


// =======================================================================================
