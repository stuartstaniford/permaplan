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
