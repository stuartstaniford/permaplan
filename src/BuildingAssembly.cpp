// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// Functionality common to building sub-assemblies such as gables, sheds, etc.  At the 
// moment, largely has functionality for managing an array of BuildingRects

#include "BuildingAssembly.h"
#include "BuildingRect.h"

// =======================================================================================
/// @brief Constructor
/// @param N the number of BuildingRect elements in this particular assembly

BuildingAssembly::BuildingAssembly(int N):
                                  nRects(N)
{
  rects = new BuildingRect[nRects];
}


// =======================================================================================
/// @brief Destructor

BuildingAssembly::~BuildingAssembly(void)
{
  delete rects;
}


// =======================================================================================
/// @brief This buffers the geometry of the component parts of the assembly
/// /// 
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).

bool BuildingAssembly::bufferGeometryOfObject(TriangleBuffer* T)
{
  for(int i=0; i<nRects; i++)
  unless(rects->bufferGeometryOfElement(T, position))
    return false;

  return true;
}


// =======================================================================================
