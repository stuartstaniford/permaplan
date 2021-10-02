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
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).
/// @param offset A vec3 of the position of this element's containing object, thus 
/// allowing the absolute position of the element to be computed (since we only know our
/// relative position).

bool TreePart::bufferGeometry(TriangleBuffer* T, vec3 offset)
{
  // Add nothing in this base class
  err(-1, "Bad call to TreePart::bufferGeometry");
}


// =======================================================================================
/// @brief The amount of vertex and index space we would need in a triangle buffer.
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.

void TreePart::triangleBufferSizesRecurse(unsigned& vCount, unsigned& iCount)
{
  // Add nothing in this base class
  err(-1, "Bad call to TreePart::triangleBufferSizesRecurse");
}


// =======================================================================================
/// @brief Decide if a ray touches us.  
/// 
///Subclassses need to implement this so we can tell whether the mouse
/// @returns True if the ray touches us, otherwise false.
/// @param position The vec3 for a point on the ray to be matched.
/// @param direction The vec3 for the direction of the ray.
/// @param offset A vec3 which gives the position of this element relative to its
/// containing object (since elements generally have relative positions, this is needed
/// to compute absolute position matches).

bool TreePart::matchRay(vec3& position, vec3& direction, vec3 offset)
{
  err(-1, "Bad call to TreePart::matchRay");
}


// =======================================================================================
/// @brief Function to validate this kind of object.  
/// 
/// Subclasses must implement.
/// @param l The recursive depth in the validation search

void TreePart::selfValidate(unsigned l)
{
#ifdef LOG_TREE_VALIDATION
  err(-1, "Bad call to TreePart::selfValidate");
#endif
}


// =======================================================================================
/// @brief Grow the tree by a certain number of years (possibly fractional).
/// @param years The number of years to grow the tree by.


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
/// @brief Tell callers our name at runtime.
/// @returns A const pointer to a C-string with the name.

const char* TreePart::objectName(void)
{
  static char* name = (char*)"Abstract TreePart";
  return name;
}


// =======================================================================================
/// @brief Interface for getting a row in an HTML table about this part.
/// 
/// We assume we are part of a table of tree parts and we just contribute one row
/// about this particular object.  Subclasses need to provide a working implementation.  
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server
// 

bool TreePart::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("<tr><td>Abstract TreePart</td>");
  return true;
}


// =======================================================================================
