// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is an abstract interface for all parts of trees.

#include "TreePart.h"
#include "HttpDebug.h"
#include <err.h>


// =======================================================================================
/// @brief Constructor.
/// @param treeIndex Index of our tree in the static structures in Tree

TreePart::TreePart(unsigned short treeIndex):
                                      ourTreeIndex(treeIndex)
{
}


// =======================================================================================
/// @brief Destructor

TreePart::~TreePart(void)
{
}


// =======================================================================================
/// @brief Buffer the vertices/indices for this part - stub here, as base class for interface.

bool TreePart::bufferGeometry(TriangleBuffer* T, vec3 offset)
{
  // Add nothing in this base class
  err(-1, "Bad call to TreePart::bufferGeometry");
}


// =======================================================================================
// Assess the number of vertices/indices required in a triangle buffer to render us
// and all our children.

void TreePart::triangleBufferSizesRecurse(unsigned& vCount, unsigned& iCount)
{
  // Add nothing in this base class
  err(-1, "Bad call to TreePart::triangleBufferSizesRecurse");
}


// =======================================================================================
// Match a ray.  Subclassses need to implement this so we can tell whether the mouse
// is over the tree or not.

bool TreePart::matchRay(vec3& position, vec3& direction, vec3 offset)
{
  err(-1, "Bad call to TreePart::matchRay");
}


// =======================================================================================
// Function to validate this kind of object.  Subclasses must implement.

#ifdef LOG_TREE_VALIDATION

void TreePart::selfValidate(unsigned l)
{
  err(-1, "Bad call to TreePart::selfValidate");
}

#endif


// =======================================================================================
// Function that is applied to grow the tree by a certain number of years (possibly
// fractional).

void TreePart::growStep(float years)
{
  err(-1, "Bad call to TreePart::growStep");
}

// =======================================================================================
// We cannot update the bounding box at this virtual level and should never be called.

 bool TreePart::updateBoundingBox(BoundingBox* box, float altitude)
{
  err(-1, "Bad call to TreePart::updateBoundingBox");
  return false;
}


// =======================================================================================
// Tell callers our name at runtime.

const char* TreePart::objectName(void)
{
  static char* name = (char*)"Abstract TreePart";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.

bool TreePart::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("<tr><td>Abstract TreePart</td>");
  return true;
}


// =======================================================================================
