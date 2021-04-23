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
// This function is used te ensure the stability of f1, f2 as we go up the tube, we use the 
// last f1,f2 as the starting point for the next set.  Eg if sides == 8 the cross section 
// of the PathTube which points in the instantaneous dir direction looks like this (dir 
// points into the screen):
/*
     ^
     | f1 direction
 
   -----
  /     \
 /       \
|         |  ---->
|         |  f2 direction
 \       /
  \     /
   -----
*/

void updateCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius)
{
  glm_vec3_cross(dir, f1, f2);
  glm_vec3_cross(f2, dir, f1);
  glm_vec3_scale_as(f1, radius, f1);
  glm_vec3_scale_as(f2, radius, f2);
}


// =======================================================================================
// This is where the actual geometry is defined - we render it into a buffer on request
// N.B. OpenGL faceculling: triangles are counterclockwise viewed from outside the tube

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
  
  // XX need to add offset in here
  
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

      // Now compute the indices
      indices[index]   = vOffset;     // triangle base is bottom vertex
      indices[index+1] = vOffset+1+j; // the j vertex
      indices[index+2] = vOffset+1+(j+sides-1)%sides; // the j-1 vertex (triangle is counterclockwise)

      //Increment vertex, index before next vertex/triangle
      index+=3;
      vertex++;
     }
    startRow = 1u;  // Because we already did one row
    vOffset += 1u; //  We used up a slot of the vertex space, so indices now need to allow for that
   }
  else
    startRow = 0u;

  // Need to know how many rows of tube sides to do, based on whether there is a closed top or not
  if(closedTop)
    endRow = NPath-1;
  else
    endRow = NPath;
  
  // Loop over the rows of tube sides
  for(int i=startRow; i<endRow; i++)
   {
    // Compute the local tangent to the path at the second point in the path
    if(i>1)
     {
      glm_vec3_sub(path[i], path[i-1], firstDelt);
      glm_vec3_sub(path[i+1], path[i], secondDelt);
      glm_vec3_add(firstDelt, secondDelt, localAxis); // don't care about scale of this
      updateCrossVectors(localAxis, f1, f2, path[i][3]);  //fourth float of vec4 path is radius
     }
    else
     {
      glm_vec3_sub(path[i+1], path[i], localAxis);
      getCrossVectors(localAxis, f1, f2, path[i][3]);  //fourth float of vec4 path is radius
     }
    
    for(int j=0; j<sides; j++)
     {
      // Compute the j'th vertex around 
      ang = j*angleRadians;
      cosAng = cosf(ang);
      sinAng = sinf(ang);     
      for(int m=0; m<3; m++)
        norm[m] = cosAng*f1[m] + sinAng*f2[m]; // scaled to radius
      glm_vec3_add(path[i+startRow], norm, point);
      vertices[vertex].setPosition(point);
      vertices[vertex].setColor(color);
      vertices[vertex].setNormal(norm); // to be normalized in gpu

      if(i>1) // if !closedBase, need to do first row of vertices without doing triangles
       {
        // Now compute the indices for the first triangle.  This is between us and the prior
        // row, first triangle has two vertices in prior, one in this
        indices[index]   = vOffset + i*sides + j;                     // new vertex we just added
        indices[index+1] = vOffset + (i-1)*sides + (j+sides-1)%sides; // prior row, j one less
        indices[index+2] = vOffset + (i-1)*sides + j;                 // same vertex in prior row

        // Now compute the indices for the second triangle, which has two vertices in this row
        // one in the prior row
        indices[index+3]  = vOffset + i*sides + j;                     // new vertex we just added
        indices[index+4]  = vOffset + i*sides + (j+sides-1)%sides;     // this row, j one less
        indices[index+5]  = vOffset + (i-1)*sides + (j+sides-1)%sides; // prior row, j one less

        //Increment index before next vertex/triangle
        index+=6;
       }
      vertex++;   //Increment vertex before next vertex/triangle
     }
   }
  
  // Now deal with the single vertex and cone of triangles at the closed top if present.
  if(closedTop)
   {
    // Create the vertex for the top of the cone
    glm_vec3_sub(path[NPath-1], path[NPath-2], norm);
    vertices[vertex].setPosition(path[NPath-1]);
    vertices[vertex].setColor(color);
    vertices[vertex].setNormal(norm); // to be normalized in gpu

    unsigned nRows = endRow-startRow;
    for(int j=0; j<sides; j++)
     {
      // Compute the indices round each cone triangle 
      indices[index]   = vOffset + nRows*sides;         // new top of cone vertex we just added
      indices[index+1] = vOffset + (nRows-1)*sides + (j+sides-1)%sides; // last row, j one less
      indices[index+2] = vOffset + (nRows-1)*sides + j;                 // last row, this j
      index+=3;
     }
   }

  // Phew!! Go home! 
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
  float     angleRadians  = 2.0f*M_PI/sides;
  unsigned  startRow, endRow;
  float ang, cosAng, sinAng;
  
  vec3 localAxis, firstDelt, secondDelt, norm, f1, f2, point;
  
  // bottom end point
  if(closedBase)
   {
    // Make the single vertex at the bottom of the closed tube.
    glm_vec3_add(path[0], offset, point);
    if(box->extends(point))
      retVal = true;
    startRow = 1u;  // Because we already did one row
   }
  else
    startRow = 0u;

  // Handle the single vertex at top if there is a closed top
  if(closedTop)
   {    
    glm_vec3_add(path[NPath-1], offset, point);
     if(box->extends(point))
       retVal = true;
    endRow = NPath-1;
   }
  else
    endRow = NPath;
  
  // Loop over the rows of tube sides
  for(int i=startRow; i<endRow; i++)
   {
    // Compute the local tangent to the path at the second point in the path
    if(i>1)
     {
      glm_vec3_sub(path[i], path[i-1], firstDelt);
      glm_vec3_sub(path[i+1], path[i], secondDelt);
      glm_vec3_add(firstDelt, secondDelt, localAxis); // don't care about scale of this
      updateCrossVectors(localAxis, f1, f2, path[i][3]);  //fourth float of vec4 path is radius
     }
    else
     {
      glm_vec3_sub(path[i+1], path[i], localAxis);
      getCrossVectors(localAxis, f1, f2, path[i][3]);  //fourth float of vec4 path is radius
     }
    
    for(int j=0; j<sides; j++)
     {
      // Compute the j'th vertex around 
      ang = j*angleRadians;
      cosAng = cosf(ang);
      sinAng = sinf(ang);     
      for(int m=0; m<3; m++)
       {
        norm[m] = cosAng*f1[m] + sinAng*f2[m]; // scaled to radius
        point[m] = path[i+startRow][m] + norm[m] + offset[m];
       }
      if(box->extends(point))
        retVal = true;
     }
   }
  
  return retVal;
}


// =======================================================================================
