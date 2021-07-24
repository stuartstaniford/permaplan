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
/// @returns True if the bounding box was changed in anyway, false otherwise.
/// @param box The bounding box that is to be updated.
/// @param offset The vec3 of our offset from a containing object.
/// @todo Comment from VisualElement: "Note that subclass version of this must also 
/// take on the responsibility of notifying our qTreeNode if we've changed our extent."
/// This has not been understood or addressed.

bool BuildingRect::updateBoundingBox(BoundingBox* box, vec3 offset)
{
  bool retVal = false;
  vec3 base, point;
  
  glm_vec3_add(relativePos, offset, base);
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
/// @brief This is where the actual geometry is defined - we render it into a buffer on 
/// request
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).
/// @param offset A vec3 of the position of this element's containing object, thus 
/// allowing the absolute position of the element to be computed (since we only know our
/// relative position).

bool BuildingRect::bufferGeometryOfElement(TriangleBuffer* T, vec3 offset)
{
    Vertex*   vertices;
    unsigned* indices;
    unsigned  vOffset, vCount, iCount;
    
    // Get the space we need in the triangle buffer
    triangleBufferSizes(vCount, iCount);
    unless(T->requestSpace(&vertices, &indices, vOffset, vCount, iCount))
     {
      LogTriangleBufferErrs("BuildingRect TriangleBuffer request for %u,%u failed at %u.\n",
                                                            vCount, iCount, vOffset);
      return false;
     }
  
  // Compute the normal
  vec3 norm;
  if(normForward)
    glm_vec3_cross(sides[0], sides[1], norm);
  else
    glm_vec3_cross(sides[1], sides[0], norm);
    
  // Compute the vertex positions
  vec3 base, point;
  glm_vec3_add(relativePos, offset, base);
  vertices[0].setPosition(base);
  glm_vec3_add(base, sides[0], point);
  vertices[1].setPosition(point);
  glm_vec3_add(base, sides[1], point);
  vertices[2].setPosition(point);
  glm_vec3_add(point, sides[0], point);
  vertices[2].setPosition(point);

  // Set everything else which is the same for all four vertices.
  for(int i=0; i<4; i++)
   {
    vertices[i].setColor(color);
    vertices[i].setNormal(norm);
    vertices[i].setObjectId(getObjectIndex());
   }
  
  // Do the triangle indices
  // https://learnopengl.com/Advanced-OpenGL/Face-culling
  if(normForward)
   {
    // Lower triangle
    indices[0] = vOffset + 0;
    indices[1] = vOffset + 1;
    indices[2] = vOffset + 2;
    //Upper triangle
    indices[3] = vOffset + 1;
    indices[4] = vOffset + 3;
    indices[5] = vOffset + 2;    
   }
  else
   {
    // Lower triangle
    indices[0] = vOffset + 2;
    indices[1] = vOffset + 1;
    indices[2] = vOffset + 0;
    //Upper triangle
    indices[3] = vOffset + 2;
    indices[4] = vOffset + 3;
    indices[5] = vOffset + 1;
   }
  
  return true;
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
