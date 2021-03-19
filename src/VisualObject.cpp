
// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree

#include "VisualObject.h"
#include "LandSurfaceRegion.h"
#include "Quadtree.h"

vec3 zeroVec = {0.0f, 0.0f, 0.0f};

// =======================================================================================
// Constructors

VisualObject::VisualObject(bool absHeights, BoundingBox* B):
                                box(B),
                                altitude(0.0f),
                                qTreeNode(NULL),
                                useNoTexColor(false),
                                absoluteHeights(absHeights)
{
}


// =======================================================================================
// Destructor

VisualObject::~VisualObject(void)
{
  if(box)
    delete box;
}

// =======================================================================================
// Function to validate this kind of object.  Right now, the VisualObject version just
// returns happily to cover for subclasses that haven't implemented this yet.
// XX We should make it so this err()s, and the subclasses have to to their own.

#ifdef LOG_TREE_VALIDATION

void VisualObject::selfValidate(unsigned l)
{
  return;
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
// But in general, this class should own a single row in a table, with
// the type of visual object in the first column, and any details provided
// in the second column.

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
