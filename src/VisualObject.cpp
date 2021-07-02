
// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree

#include "VisualObject.h"
#include "LandSurfaceRegion.h"
#include "Quadtree.h"
#include "ObjectGroup.h"


// =======================================================================================
// Static variables

unsigned VisualObject::nextIndex = 1;
std::unordered_map<unsigned, VisualObject*> VisualObject::allObjects;
Lockable VisualObject::staticLock;

// =======================================================================================
/// @brief Constructor for the VisualObject.
/// 
/// In addition to doing default values for the object state, we also allocate it a unique
/// index and keep track of that in the allObjects container.
/// 
/// @param absHeights This is true if the VisualObject should have absolute height, false
/// if they should be relative to the land surface.
/// @param B A pointer to a bounding box.  This defaults to NULL and that is the usual 
/// case - the object will allocate it's own BoundingBox.  But if there's a reason the
/// object should use a different bounding box, it is supplied here.
///  

VisualObject::VisualObject(bool absHeights, BoundingBox* B):
                                box(B),
                                label(NULL),
                                qTreeNode(NULL),
                                groupOwner(NULL),
                                altitude(0.0f),
                                isGroup(false),
                                useNoTexColor(false),
                                absoluteHeights(absHeights)
{
  staticLock.lock();
  objIndex = nextIndex++;
  allObjects[objIndex] = this;
  staticLock.unlock();
}


// =======================================================================================
/// @brief Destructor
///
/// Deletes box (the bounding box pointer), it's label (a char array on the heap), 
/// and erases its objIndex from the allObjects container.

VisualObject::~VisualObject(void)
{
  LogObjectCreation("Object destroyed with id %u.\n", objIndex);
  if(box)
    delete box;
  if(label)
    delete[] label;
  
  staticLock.lock();
  allObjects.erase(objIndex);
  staticLock.unlock();
}


// =======================================================================================
/// @brief Sets the label for the VisualObect.
///
/// Function to set the label of this object (not normally over-ridden).  Makes a copy
/// on the heap (and will delete the last version if called more than once).
/// 
/// @param inLabel - supplied label.  Will be copied and not relied on after this call.

void VisualObject::setLabel(const char* inLabel)
{
  if(label)
    delete[] label;
  label = new char[strlen(inLabel)+1];
  strcpy(label, inLabel);
}


// =======================================================================================
// Function to validate this kind of object.  This needs to be over-ridden by subclasses.

#ifdef LOG_TREE_VALIDATION

void VisualObject::selfValidate(unsigned l)
{
  err(-1, "Call to unimplemented VisualObject::selfValidate.\n");
}

#endif

// =======================================================================================
// Function to set our altitude based on the land height where we are.
// This version can be overwritten by implementing subclasses, but should do the
// right thing in most cases.

void VisualObject::setAltitude(LandSurfaceRegion* surface)
{
  unless(absoluteHeights)
   {
    float x,y;
    getGroundContact(x,y);
    altitude = surface->getAltitude(x,y);
    LogObjectAltitudes("Object of type %s at %.2f,%.2f has altitude %.2f.\n",
                       objectName(), x, y, altitude);
   }
  else
    LogObjectAltitudes("Object of type %s has absolute altitudes.\n", objectName());
}


// =======================================================================================
// Function to return the location at which we officially contact ground/grade level.
// This version in this class finds the midpoint of the bounding box.
// Subclasses may want to override this.

void VisualObject::getGroundContact(float& x, float& y)
{
  unless(box)
    err(-1, "Called VisualObject::getGroundContact with no bounding box.");

  x = (box->lower[0] + box->upper[0])/2.0f;
  y = (box->lower[1] + box->upper[1])/2.0f;
}


// =======================================================================================
// Function to remove ourself from the quadtree

void VisualObject::removeFromQuadtree(void)
{
 {if(qTreeNode) 
   qTreeNode->removeVisualObject(this);}
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool VisualObject::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  err(-1, "Call to unimplemented VisualObject::getNextUniqueVertex.\n");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool VisualObject::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  err(-1, "Call to unimplemented VisualObject::getNextVertex.\n");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

int VisualObject::getNextIndex(bool resetToFirst)
{
  err(-1, "Call to unimplemented VisualObject::getNextIndex.\n");
}


// =======================================================================================
// Used when this visual object is going to be a visual indicator with a fixed color.
// XX - should this be hived off into a different class, that could use multiple
// inheritance to pull in, so this isn't present in non-visual indicator subclasses?

void VisualObject::setNoTexColor(unsigned color)
{
  noTexColor    = color;
  useNoTexColor = true;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool VisualObject::bufferGeometry(TriangleBuffer* T)
{
  err(-1, "Called unimplemented superclass VisualObject::bufferGeometry.\n");
  return false;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool VisualObject::bufferGeometry(TriangleBuffer* T, vec3 offset)
{
  err(-1, "Called unimplemented superclass VisualObject::bufferGeometry.\n");
  return false;
}


// =======================================================================================
// Decide if a ray touches us.  This version will work in many cases, but there may
// be more efficient methods possible for specific subclasses.  This will only work
// if the subclass has implemented getNextVertex.

bool VisualObject::matchRay(vec3& position, vec3& direction, float& lambda)
{
  unless(box->matchRay(position, direction, lambda))
    return false;
  return VisualElement::matchRay(position, direction, lambda, zeroVec);
}


// =======================================================================================
// Stub don't use

bool VisualObject::matchRay(vec3& position, vec3& direction, float& lambda, vec3 offset)
{
  err(-1, "Called unimplemented superclass VisualObject::matchRay with offset.\n");

}


// =======================================================================================
// Stub don't use

float VisualObject::estimateOpacity(vec3 direction)
{
  err(-1, "Called unimplemented superclass VisualObject::estimateTransparency.\n");
}


// =======================================================================================
// Compute the bounding box.  This version will work in many cases, but there may
// be more efficient methods possible for specific subclasses.  This will only work
// if the subclass has implemented getNextUniqueVertex.

// Note that subclass version of this must also take on the responsibility of notifying
// our qTreeNode if we've changed our extent.

void VisualObject::updateBoundingBox(void)
{
  bool boxChanged = false;
  if(!box)
   {
    box = new BoundingBox();
    boxChanged = true;
   }
  else
    box->hugeValify();

  VisualElement::updateBoundingBox(box, zeroVec);
  if(boxChanged && qTreeNode)
    qTreeNode->notifyObjectBoxChange(this);
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void VisualObject::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  err(-1, "Called unimplemented superclass VisualObject::triangleBufferSizes.\n");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

const char* VisualObject::objectName(void)
{
  err(-1, "Called unimplemented superclass VisualObject::objectName.\n");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses
// But in general, this class should provide a whole page with details about the 
// object.

bool VisualObject::diagnosticHTML(HttpDebug* serv)
{
  err(-1, "Called unimplemented superclass VisualObject::diagnosticHTML.\n");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses
// But in general, this class should own a single row in a table, with
// the type of visual object in the first column, and any details provided
// in the second column.  A link should be provided to whatever URL serves
// diagnosticHTML (above).

bool VisualObject::diagnosticHTMLSummary(HttpDebug* serv)
{
  err(-1, "Called unimplemented superclass VisualObject::diagnosticHTMLSummary.\n");
}


// =======================================================================================
// This provides access to the diagnosticHTML page of any object via it's objIndex


bool VisualObject::diagnosticHTMLSelection(HttpDebug* serv, char* path)
{
  unsigned index = atoi(path);
  staticLock.lock();
  if(allObjects.count(index))
   {
    VisualObject* V = allObjects[index];
    staticLock.unlock();
    return V->diagnosticHTML(serv);
   }
  else
   {
    staticLock.unlock();
    LogRequestErrors("Request for unknown visual object index: %s\n", path);
    return false;
   }
}


// =======================================================================================
