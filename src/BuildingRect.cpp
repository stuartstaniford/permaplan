// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class is used extensively to represent pieces of buildings, such as a wall or
// one plane of a roof.  It provides for keeping track of its orientation and size
// relative to the larger structure, and for keeping track of texture.


#include "BuildingRect.h"


// =======================================================================================
/// @brief Constructor

BuildingRect::BuildingRect(void)
{
}


// =======================================================================================
/// @brief Destructor

BuildingRect::~BuildingRect(void)
{
}


// =======================================================================================
/// @brief Updates a bounding box.
/// 
/// Function to update an axis-aligned bounding box.    
/// @todo XX Comment from VisualElement: "Note that subclass version of this must also 
/// take on the responsibility of notifying our qTreeNode if we've changed our extent."
/// This has not been understood or addressed.
/// @returns True if the bounding box was changed in anyway, false otherwise.
/// @param box The bounding box that is to be updated.
/// @param offset The vec3 of our offset from a containing object.

bool BuildingRect::updateBoundingBox(BoundingBox* box, vec3 offset)
{
  bool retVal = false;
  vec3 base, point;
  
  glm_vec3_add(pos, offset, base);
  retVal |= box->extends(base);
  
  glm_vec3_add(base, sides[0], point);
  retVal |= box->extends(point);
  
  glm_vec3_add(base, sides[1], point);
  retVal |= box->extends(point);
  
  glm_vec3_add(point, sides[0], point);
  retVal |= box->extends(point);

  return retVal;
}


// =======================================================================================
/// @brief How much space we need in a TriangleBuffer on a call to bufferGeometry
///
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.

void BuildingRect::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 4u;
  iCount = 6u;
  LogTriangleBufEstimates("BuildingRect TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
