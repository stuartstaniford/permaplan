// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Class for rendering an element that consists of a tube (of varying width) 
// that follows a specified path (set of points).  Provides support to things 
// like semicircular arrows.

#include "PathTube.h"
#include "CircleIterator.h"
#include "BoundingBox.h"
#include <assert.h>


// =======================================================================================
/// @brief Constructor

PathTube::PathTube(void):
                  path(NULL),
                  NPath(0u),
                  sides(10)  // sensible default
{  
}


// =======================================================================================
/// @brief Destructor

PathTube::~PathTube(void)
{
}


// =======================================================================================
/// @brief How much space we need in a TriangleBuffer
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.

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
    iCount -= 3*sides;
   }

  LogTriangleBufEstimates("PathTube TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
/// @brief This is where the actual geometry is defined - we render it into a buffer 
/// on request.
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).
/// @param offset A vec3 of the position of this element's containing object, thus 
/// allowing the absolute position of the element to be computed (since we only know our
/// relative position).

// N.B. OpenGL faceculling: triangles are counterclockwise viewed from outside the tube

bool PathTube::bufferGeometryOfElement(TriangleBuffer* T, vec3 offset)
{
  float     angleDegrees  = 360.0f/sides;
  Vertex*   vertices;
  unsigned* indices;
  unsigned  vOffset, vCount, iCount;
  unsigned  startRow, endRow;
  
  triangleBufferSizes(vCount, iCount);
  unless(T->requestSpace(&vertices, &indices, vOffset, vCount, iCount))
   {
    LogTriangleBufferErrs("PathTube TriangleBuffer request for %u,%u failed at %u.\n",
                                                    vCount, iCount, vOffset);
    return false;
   }

  unsigned        vertex = 0u;
  unsigned        index = 0u;
  vec3            localAxis, firstDelt, secondDelt, norm;
  CircleIterator  circIter;
  
  if(closedBase)
   {
    // Make the single vertex at the bottom of the closed tube.
    glm_vec3_add(path[0], offset, vertices[vertex].pos);
    vertices[vertex].setColor(color);
    glm_vec3_sub(path[0], path[1], norm);
    vertices[vertex].setNormal(norm); // to be normalized in gpu
    vertices[vertex].setObjectId(getObjectIndex());
    LogPathTubeBuffer("Pathtube: Added base vertex %u at [%.1f, %.1f, %.1f]\n", 
                      vertex + vOffset, vertices[vertex].pos[0], vertices[vertex].pos[1],
                      vertices[vertex].pos[2]);
    vertex++;
    
    // Compute the local tangent to the path at the second point in the path
    glm_vec3_sub(path[1], path[0], firstDelt);
    glm_vec3_sub(path[2], path[1], secondDelt);
    glm_vec3_add(firstDelt, secondDelt, localAxis); // don't care about scale of this
    
    circIter.update(path[1], localAxis, path[1][3], offset);

    // Loop over triangles from the single vertex up to the first ring of tube vertices.
    for(int j=0; j<sides; j++)
     {
      // Compute the j'th vertex around 
      circIter.getPoint(j*angleDegrees, vertices[vertex].pos, norm);
      vertices[vertex].setColor(color);
      vertices[vertex].setNormal(norm); // to be normalized in gpu
      vertices[vertex].setObjectId(getObjectIndex());
      LogPathTubeBuffer("Pathtube: Added first row vertex %u at [%.1f, %.1f, %.1f]\n", 
                        vertex + vOffset, vertices[vertex].pos[0], vertices[vertex].pos[1],
                        vertices[vertex].pos[2]);

      // Now compute the indices
      indices[index]   = vOffset;     // triangle base is bottom vertex
      indices[index+1] = vOffset+1+j; // the j vertex
      indices[index+2] = vOffset+1+(j+sides-1)%sides; // the j-1 vertex (triangle is counterclockwise)
      LogPathTubeBuffer("Pathtube: Added base triangle %u: %u, %u, %u\n", index/3,
                        vOffset, vOffset+1+j, vOffset+1+(j+sides-1)%sides);

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
  endRow = NPath;
  if(closedBase)
    endRow--;
  if(closedTop)
    endRow--;
  
  // Loop over the rows of tube sides
  for(int i=startRow; i<endRow; i++)
   {
    // Compute the local tangent to the path at the second point in the path
    if(i==0)
      glm_vec3_sub(path[i+1], path[i], localAxis);
    else if(i==NPath-1)      
      glm_vec3_sub(path[i], path[i-1], localAxis);
    else
     {
      glm_vec3_sub(path[i], path[i-1], firstDelt);
      glm_vec3_sub(path[i+1], path[i], secondDelt);
      glm_vec3_add(firstDelt, secondDelt, localAxis); // don't care about scale of this
     }
    circIter.update(path[i], localAxis, path[i][3], offset);

    for(int j=0; j<sides; j++)
     {
      // Compute the j'th vertex around 
      circIter.getPoint(j*angleDegrees, vertices[vertex].pos, norm);
      vertices[vertex].setColor(color);
      vertices[vertex].setNormal(norm); // to be normalized in gpu
      vertices[vertex].setObjectId(getObjectIndex());
      LogPathTubeBuffer("Pathtube: Added %u row vertex %u at [%.1f, %.1f, %.1f]\n", i,
                        vertex + vOffset, vertices[vertex].pos[0], vertices[vertex].pos[1],
                        vertices[vertex].pos[2]);

      if(i>0) // if !closedBase, need to do first row of vertices without doing triangles
       {
        // Now compute the indices for the first triangle.  This is between us and the prior
        // row, first triangle has two vertices in prior, one in this
        indices[index]   = vOffset + i*sides + j;                     // new vertex we just added
        indices[index+1] = vOffset + (i-1)*sides + (j+sides-1)%sides; // prior row, j one less
        indices[index+2] = vOffset + (i-1)*sides + j;                 // same vertex in prior row
        LogPathTubeBuffer("Pathtube: Added side triangle %u: %u, %u, %u\n", index/3,
                          vOffset + i*sides + j, vOffset + (i-1)*sides + (j+sides-1)%sides,
                          vOffset + (i-1)*sides + j);

        // Now compute the indices for the second triangle, which has two vertices in this row
        // one in the prior row
        indices[index+3]  = vOffset + i*sides + j;                     // new vertex we just added
        indices[index+4]  = vOffset + i*sides + (j+sides-1)%sides;     // this row, j one less
        indices[index+5]  = vOffset + (i-1)*sides + (j+sides-1)%sides; // prior row, j one less
        LogPathTubeBuffer("Pathtube: Added side triangle %u: %u, %u, %u\n", index/3+1,
                          vOffset + i*sides + j, vOffset + i*sides + (j+sides-1)%sides,
                          vOffset + (i-1)*sides + (j+sides-1)%sides);

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
    glm_vec3_add(path[NPath-1], offset, vertices[vertex].pos);
    vertices[vertex].setColor(color);
    vertices[vertex].setNormal(norm); // to be normalized in gpu
    vertices[vertex].setObjectId(getObjectIndex());
    LogPathTubeBuffer("Pathtube: Added top vertex %u at [%.1f, %.1f, %.1f]\n",
                      vertex + vOffset, vertices[vertex].pos[0], vertices[vertex].pos[1],
                      vertices[vertex].pos[2]);

    unsigned nRows = endRow-startRow;
    for(int j=0; j<sides; j++)
     {
      // Compute the indices round each cone triangle 
      indices[index]   = vOffset + nRows*sides;         // new top of cone vertex we just added
      indices[index+1] = vOffset + (nRows-1)*sides + (j+sides-1)%sides; // last row, j one less
      indices[index+2] = vOffset + (nRows-1)*sides + j;                 // last row, this j
      LogPathTubeBuffer("Pathtube: Added top cone triangle %u: %u, %u, %u\n", index/3,
                        vOffset + nRows*sides, vOffset + (nRows-1)*sides + (j+sides-1)%sides,
                        vOffset + (nRows-1)*sides + j);
      index+=3;
     }
   }

  // Phew!! Go home! 
  return true;
}


// =======================================================================================
/// @brief Decide if a ray touches us.  
/// @returns True if the ray touches us, otherwise false.
/// @param position The vec3 for a point on the ray to be matched.
/// @param direction The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.
/// @param offset A vec3 which gives the position of this element relative to its
/// containing object (since elements generally have relative positions, this is needed
/// to compute absolute position matches).
/// @todo The implementation of this appears to be incomplete and non-functional.

bool PathTube::matchRayToElement(vec3& position, vec3& direction, float& lambda, vec3 offset)
{
  float     angleRadians  = 2.0f*M_PI/sides;
  unsigned  startRow, endRow;
  float     ang, cosAng, sinAng;  
  vec3      localAxis, firstDelt, secondDelt, norm, f1, f2, point;
  //vec3      triangle[3];
  
  if(closedBase)
   {
    // Make the single vertex at the bottom of the closed tube.
    glm_vec3_add(path[0], offset, point);
    //XX set triangle here
    
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
       {
        norm[m] = cosAng*f1[m] + sinAng*f2[m]; // scaled to radius
        point[m] = path[1][m] + norm[m] + offset[m];
       }
      //set the triangle here      
      //indices[index]   = vOffset;     // triangle base is bottom vertex
      //indices[index+1] = vOffset+1+j; // the j vertex
      //indices[index+2] = vOffset+1+(j+sides-1)%sides; // the j-1 vertex (triangle is counterclockwise)
     }
    startRow = 1u;  // Because we already did one row
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
       {
        norm[m] = cosAng*f1[m] + sinAng*f2[m]; // scaled to radius
        point[m] = path[i+startRow][m] + norm[m] + offset[m];
       }
      // set triangle here
      
      if(i>1) // if !closedBase, need to do first row of vertices without doing triangles
       {
        // Now compute the indices for the first triangle.  This is between us and the prior
        // row, first triangle has two vertices in prior, one in this
        //indices[index]   = vOffset + i*sides + j;                     // new vertex we just added
        //indices[index+1] = vOffset + (i-1)*sides + (j+sides-1)%sides; // prior row, j one less
        //indices[index+2] = vOffset + (i-1)*sides + j;                 // same vertex in prior row

        // Now compute the indices for the second triangle, which has two vertices in this row
        // one in the prior row
        //indices[index+3]  = vOffset + i*sides + j;                     // new vertex we just added
        //indices[index+4]  = vOffset + i*sides + (j+sides-1)%sides;     // this row, j one less
        //indices[index+5]  = vOffset + (i-1)*sides + (j+sides-1)%sides; // prior row, j one less

        //Increment index before next vertex/triangle
       }
     }
   }
  
  // Now deal with the single vertex and cone of triangles at the closed top if present.
  if(closedTop)
   {
    // Create the vertex for the top of the cone
    glm_vec3_sub(path[NPath-1], path[NPath-2], norm);
    glm_vec3_add(path[NPath-1], offset, point);
    // set the triangle here
    
    //unsigned nRows = endRow-startRow;
    for(int j=0; j<sides; j++)
     {
      // Compute the indices round each cone triangle 
      //indices[index]   = vOffset + nRows*sides;         // new top of cone vertex we just added
      //indices[index+1] = vOffset + (nRows-1)*sides + (j+sides-1)%sides; // last row, j one less
      //indices[index+2] = vOffset + (nRows-1)*sides + j;                 // last row, this j
     }
   }

  return false;
}


// =======================================================================================
/// @brief Updates the bounding box.
/// 
/// Function to update an axis-aligned bounding box.    
/// @returns True if the bounding box was changed in anyway, false otherwise.
/// @param box The bounding box that is to be updated.
/// @param offset The vec3 of our offset from a containing object.

bool PathTube::updateBoundingBox(BoundingBox* box, vec3 offset)
{
  bool      retVal        = false;
  float     angleDegrees  = 360.0f/sides;
  unsigned  startRow, endRow;  
  vec3 localAxis, firstDelt, secondDelt, norm, point;
  
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
  CircleIterator circIter;
  for(int i=startRow; i<endRow; i++)
   {
    // Compute the local tangent to the path at the second point in the path
    if(i==0)
      glm_vec3_sub(path[i+1], path[i], localAxis);
    else if(i==NPath-1)
      glm_vec3_sub(path[i], path[i-1], localAxis);
    else
     {
      glm_vec3_sub(path[i], path[i-1], firstDelt);
      glm_vec3_sub(path[i+1], path[i], secondDelt);
      glm_vec3_add(firstDelt, secondDelt, localAxis); // don't care about scale of this
     }
    
    // XX use of i here is inconsistent
    
    circIter.update(path[i], localAxis, path[i][3], offset);
    
    for(int j=0; j<sides; j++)
     {
      // Compute the j'th vertex around 
      circIter.getPoint(j*angleDegrees, point, norm);
      if(box->extends(point))
        retVal = true;
     }
   }
  
  return retVal;
}


// =======================================================================================
