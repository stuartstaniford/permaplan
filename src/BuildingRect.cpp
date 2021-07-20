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
