// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is for a group of objects that are all to be displayed and manipulated as
// a unit (and appear in the Quadtree together).

#include "ObjectGroup.h"


// =======================================================================================
/// @brief Constructor
/// @param firstObject A pointer to the first VisualObject to be included in this 
/// ObjectGroup.

ObjectGroup::ObjectGroup(VisualObject* firstObject):
                        VisualObject(false)
{
  isGroup = true;
  insert(firstObject);
  firstObject->groupOwner = this;
  updateBoundingBox();
  LogGroupAdditions("Creating new object group with object of type %s.\n", 
                                                                firstObject->objectName());
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
}


// =======================================================================================
/// @brief Destructor

ObjectGroup::~ObjectGroup(void)
{
}


// =======================================================================================
/// @brief Add an additional object into this group
/// @param nextObject A pointer to the additional VisualObject to be included in this 
/// ObjectGroup.

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
/// @brief Update a supplied bounding box to include the union of all our member 
/// bounding boxes
/// @returns True if the boundingBox was increased, false otherwise.
/// @param B The bounding box to be updated.
/// @param offset A vec3 offset to be added to our position before doing the update.

bool ObjectGroup::updateBoundingBox(BoundingBox* B, vec3 offset)
{
  bool retVal = false;
  
  for(VisualObject* V: *this)
    if(V->updateBoundingBox(B, offset))
      retVal = true;
  
  return retVal;
} 


// =======================================================================================
/// @brief Recurse into all our parts and update our own bounding box (which we inherit
/// from VisualObject.

void ObjectGroup::updateBoundingBox(void)
{  
  if(!box)
    box = new BoundingBox();
  for(VisualObject* V: *this)
    *box += *(V->box);
}


// =======================================================================================
/// @brief Decide if a ray touches us.  
/// 
/// This method involves checking all the kids and return the closest that touches the 
/// ray (if any).
/// @returns True if the ray touches us, otherwise false.
/// @param position The vec3 for a point on the ray to be matched.
/// @param direction The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.

bool ObjectGroup::matchRayToObject(vec3& position, vec3& direction, float& lambda)
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
    if(V->matchRayToObject(position, direction, thisLambda))
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
/// @brief Validate the state of us and our children.
/// 
/// This function is called as part of validating the quadtree and visual objects in it.
/// @param l The level of checking (recursively in the quadtree).  We increase the level
/// by one when recursing into our children.

void ObjectGroup::selfValidate(unsigned l)
{
#ifdef LOG_TREE_VALIDATION
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
#endif
}



// =======================================================================================
/// @brief Tell callers our name at runtime.
/// @returns A const pointer to a C-string with the object name "Object Group".

const char* ObjectGroup::objectName(void)
{
  static char* name = (char*)"Object Group";
  return name;
}


// =======================================================================================
/// @brief Search for objects with a particular name.
/// 
/// This is a helper function for Quadtree::quadSearchHTML.  Check the documentation for
/// that function to better understand the context for this.
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server
/// @param nextRow A reference to an integer storing the next row number to write (in an
/// HTML table).
/// @param searchTerm A C-string of the object name we are looking for.
/// @param quadPath A C-string of the path within the quadtree url space (to be used in
/// constructing links to any objects found).

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
/// @brief Provide a diagnostic page about this group.
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

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
