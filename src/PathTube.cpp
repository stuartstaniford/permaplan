// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Claff for rendering an arrow that follows part of the arc of a circle.

#include "PathTube.h"
#include "AxialElement.h"
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
  vec3      localAxis, firstDelt, secondDelt, norm, f1, f2, point;
  
  if(closedBase)
   {
    // Make the single vertex at the bottom of the closed tube.
    vertices[vertex].setPosition(path[0]);
    vertices[vertex].setColor(color);
    glm_vec3_sub(path[0], path[1], norm);
    vertices[vertex].setNormal(norm); // to be normalized in gpu
    vertex++;
    
    // Compute the local tangent to the path at the second point in the path
    glm_vec3_sub(path[1], path[0], firstDelt);
    glm_vec3_sub(path[2], path[1], secondDelt);
    glm_vec3_add(firstDelt, secondDelt, localAxis); // don't care about scale of this
    
    // f1 and f2 are a basis at right angles to localAxis
    getCrossVectors(localAxis, f1, f2, path[1][3]);  //fourth float of vec4 path is radius

    // Loop over triangles from the single vertex up to the first ring of tube vertices.
    for(int j=0; j<sides; j++)
     {
      // Compute the j'th vertex around 
      ang = j*angleRadians;
      cosAng = cosf(ang);
      sinAng = sinf(ang);
      for(int m=0; m<3; m++)
        norm[m] = cosAng*f1[m] + sinAng*f2[m]; // scaled to radius
      glm_vec3_add(path[1], norm, point);
      vertices[vertex].setPosition(point);
      vertices[vertex].setColor(color);
      vertices[vertex].setNormal(norm); // to be normalized in gpu

      indices[index] = vOffset; // triangle base is bottom vertex
      index+=3;
      vertex++;
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
