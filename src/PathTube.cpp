// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Claff for rendering an arrow that follows part of the arc of a circle.

#include "PathTube.h"
#include <assert.h>

// =======================================================================================
// Constructor

PathTube::PathTube(void)
{
  NPath = 5;  //XX temp
  
  assert(NPath > 2);
}


// =======================================================================================
// Destructor

PathTube::~PathTube(void)
{
}


// =======================================================================================
// How much space we need in a TriangleBuffer

void PathTube::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = NPath*sides;  
  iCount = 6*(NPath-1)*sides;
  
  if(closedTop)
   {
    vCount -= sides-1;
    iCount -= 3*sides;
   }
  if(closedBase)
   {
    vCount -= sides-1;
    iCount -= sides;
   }

  LogTriangleBufEstimates("PathTube TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
// This is where the actual geometry is defined - we render it into a buffer on request

bool PathTube::bufferGeometry(TriangleBuffer* T, vec3 offset)
{
  float     angleRadians  = 2.0f*M_PI/sides;
  Vertex*   vertices;
  unsigned* indices;
  unsigned  vOffset, vCount, iCount;
  unsigned  startRow, endRow;
  float ang, cosAng, sinAng;
  
  triangleBufferSizes(vCount, iCount);
  unless(T->requestSpace(&vertices, &indices, vOffset, vCount, iCount))
    return false;

  unsigned vertex = 0u;
  unsigned index = 0u;
  vec3      norm;
  
  if(closedBase)
   {
    vertices[vertex].setPosition(path[0]);
    vertices[vertex].setColor(color);
    //XX set norm
    vertices[vertex].setNormal(norm); // to be normalized in gpu
    vertex++;
    for(int j=0; j<sides; j++)
     {
      ang = j*angleRadians;
      cosAng = cosf(ang);
      sinAng = sinf(ang);

      index+=3;
     }
    startRow = 1u;
   }
  else
    startRow = 0u;

  if(closedTop)
   {
    
    endRow = NPath-1;
   }
  else
    endRow = NPath;
  
  for(int i=startRow; i< endRow; i++)
   {
    for(int j=0; j<sides; j++)
     {
      ang = j*angleRadians;
      cosAng = cosf(ang);
      sinAng = sinf(ang);     
     }
   }
  
  return true;
}


// =======================================================================================
// Figure out whether a ray intersects the element or not
// https://en.wikipedia.org/wiki/Skew_lines#Distance

bool PathTube::matchRay(vec3& position, vec3& direction, float& lambda, vec3 offset)
{
  return false;
}


// =======================================================================================
// Compute the bounding box.

bool PathTube::updateBoundingBox(BoundingBox* box, vec3 offset)
{
  bool  retVal        = false;

  return retVal;
}


// =======================================================================================
