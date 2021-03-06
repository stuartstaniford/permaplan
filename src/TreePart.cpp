// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is an abstract interface for all parts of trees.

#include "TreePart.h"
#include <err.h>


// =======================================================================================
// Constructors.

TreePart::TreePart(unsigned short treeIndex):
                                      ourTreeIndex(treeIndex)
{
}


// =======================================================================================
// Destructor

TreePart::~TreePart(void)
{
}


// =======================================================================================
// Buffer the vertices/indices for this part - stub here, as base class for interface.

bool TreePart::bufferGeometry(TriangleBuffer* T, float altitude)
{
  // Add nothing in this base class
  err(-1, "Bad call to TreePart::bufferGeometry");
  return true;
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
