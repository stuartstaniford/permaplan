// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is for a group of objects that are all to be displayed and manipulated as
// a unit (and appear in the Quadtree together).


#include "ObjectGroup.h"


// =======================================================================================
// Constructor

ObjectGroup::ObjectGroup(VisualObject* firstObject):
                        VisualObject(false)
{
  isGroup = true;
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
  if(!box)
    box = new BoundingBox();
  for(VisualObject* V: *this)
    *box += *(V->box);
}


// =======================================================================================
// Decide if a ray touches us.  Check all the kids and return the closest.

bool ObjectGroup::matchRay(vec3& position, vec3& direction, float& lambda)
{
  LogGroupMatchRay("Trying to ray match group %s.\n", objectName());
  unless(box->matchRay(position, direction, lambda))
    return false;
  
  LogGroupMatchRay("Ray matched bounding box of group %s.\n", objectName());
  
  float bestLambda = HUGE_VALF;
  float thisLambda;
  bool retVal = false;
  for(VisualObject* V: *this)
   {
    if(V->matchRay(position, direction, thisLambda))
     {
      retVal = true;
      LogGroupMatchRay("Ray matched contained object (%s) in group (%s).\n", 
                       V->objectName(), objectName());
      if(thisLambda < bestLambda)
        bestLambda = thisLambda;
     }
   }
  return retVal;
}


// =======================================================================================
// Function to validate the quadtree and visual objects in it.

#ifdef LOG_TREE_VALIDATION

void ObjectGroup::selfValidate(unsigned l)
{
  LogFindGroups("Found group of type %s at level %u.\n", objectName(), l);
  unless(l)
   {
    assert(qTreeNode);
    assert(!groupOwner);
   }
  box->selfValidate(true);

  for(VisualObject* V: *this)
   {
    assert(V->groupOwner = this);
    assert(!V->qTreeNode);
    if(V->isGroup)
      V->selfValidate(l+1);
    else
      V->selfValidate(0u);
    assert(*(V->box) <= *box);
   }
}
#endif


// =======================================================================================
// Tell callers our name at runtime.

const char* ObjectGroup::objectName(void)
{
  static char* name = (char*)"Object Group";
  return name;
}


// =======================================================================================
// Helper function for Quadtree::quadSearchHTML

bool ObjectGroup::quadSearchRecursive(HttpDebug* serv, int& nextRow, 
                                                          char* searchTerm, char* quadPath)
{
  for(VisualObject* v: *this)
   {
    const char* objName = v->objectName();
    if(strcmp(objName, searchTerm)==0)
     {
      // We found one
      httPrintf("<tr><td>%d</td><td>%s</td>", nextRow, quadPath);
      httPrintf("<td>%s</td><td>Object Details</td></tr>\n", objName);
      nextRow++;
     }
    if(v->isGroup)
     {
      ObjectGroup* O = (ObjectGroup*)v;
      O->quadSearchRecursive(serv, nextRow, searchTerm, quadPath);
     }
   }
  return true;
}


// =======================================================================================
// Provide a diagnostic page about this group.

bool ObjectGroup::diagnosticHTML(HttpDebug* serv)
{
  unless(serv->startResponsePage("Object Group"))
    return false;

  // Bounding box
  unless(box->diagnosticHTML(serv))
   {
    LogResponseErrors("Couldn't output box HTTP for object group\n");
    return false;
   }
        
  // Visual objects
  unless(serv->newSection("Objects in this group"))
    return false;
  unless(serv->startTable())
    return false;
  unless(DisplayList::diagnosticHTML(serv))
   {
    LogResponseErrors("Couldn't output objects HTTP in group \n");
    return false;
   }
  httPrintf("</table></center>\n");
    
  unless(serv->endResponsePage())
    return false;
              
  return true;
}


// =======================================================================================
