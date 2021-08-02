// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// Functionality common to building sub-assemblies such as gables, sheds, etc.  At the 
// moment, has functionality for managing arrays of BuildingRects, along with some
// Rectextensions

#include "BuildingAssembly.h"
#include "BuildingRect.h"


// =======================================================================================
/// @brief Constructor
/// @param N the number of BuildingRect elements in this particular assembly

BuildingAssembly::BuildingAssembly(int nRectangles, int nExtensions):
                                  nRects(nRectangles),
                                  nExts(nExtensions)
{
  if(nRects)
    rects = new BuildingRect[nRects];
  else
    rects = NULL;
  if(nExts)
    exts = new RectExtension[nExts];
  else
    exts = NULL;
}


// =======================================================================================
/// @brief Destructor

BuildingAssembly::~BuildingAssembly(void)
{
  delete[] rects;
  delete[] exts;
}


// =======================================================================================
/// @brief How much space we need in a TriangleBuffer on a call to bufferGeometryToObject
///
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.
/// @todo Stub only at present

void BuildingAssembly::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  rects[0].triangleBufferSizes(vCount, iCount);
  vCount = nRects*vCount + nExts;  // nRects walls plus a vertex for each end cap triangle
  iCount = nRects*iCount + 3*nExts;  // nRects walls plus nExts end cap triangles
  LogTriangleBufEstimates("BuildingAssembly TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
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
    unless(rects[i].bufferGeometryOfElement(T, getPosition()))
      return false;

  return true;
}


// =======================================================================================
/// @brief Update the size of our axis-aligned bounding box.
///
/// We call each of our component BuildingRects and have them update our bounding box
/// with our position as their offset.
/// @todo - how to handle our orientation?

void BuildingAssembly::updateBoundingBox(void)
{
  unless(box)
    box = new BoundingBox();
  
  for(int i=0; i<nRects; i++)
    rects[i].updateBoundingBox(box, getPosition());
}


// =======================================================================================
/// @brief Get our position vector - has to be supplied by subclass.
///
/// @returns A float* which points to the vec3 of our position.

float* BuildingAssembly::getPosition(void)
{
  err(-1, "Call to non-implemented BuildingAssembly::getPosition.\n");  
}


// =======================================================================================
/// @brief Decide if a ray touches us.
/// 
/// This implementation works by checking each of our component BuildingRects
/// @param pos The vec3 for a point on the ray to be matched.
/// @param dir The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.
/// @todo End triangles

bool BuildingAssembly::matchRayToObject(vec3& pos, vec3& dir, float& lambda)
{
  lambda = HUGE_VAL;
  float subLambda;
  bool matched = false;
  
  for(int i=0; i<nRects; i++)
    if(rects[i].matchRayToElement(pos, dir, subLambda, getPosition()))
     {
      matched = true;
      if(subLambda < lambda)
        lambda = subLambda;
     }
  
  return matched;
}


// =======================================================================================
/// @brief Function to validate the quadtree and all the objects.

void BuildingAssembly::selfValidate(unsigned l)
{
#ifdef LOG_TREE_VALIDATION
  box->selfValidate(true);
#endif
}


// =======================================================================================
/// @brief Tell callers our name at runtime.

const char* BuildingAssembly::objectName(void)
{
  static char* name = (char*)"BuildingAssembly";
  return name;
}


// =======================================================================================
