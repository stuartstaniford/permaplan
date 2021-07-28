// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class is used extensively to represent pieces of buildings, such as a wall or
// one plane of a roof.  It provides for keeping track of its orientation and size
// relative to the larger structure, and for keeping track of texture.


#include "BuildingRect.h"


// =======================================================================================
/// @brief Constructor
/// @todo fixed color is temporary

BuildingRect::BuildingRect(void)
{
  color = 0x001177ff;
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
/// @brief Decide if a ray touches us.  
/// 
/// Function to decide whether a given line touches this particular rectangle or not (by
/// checking each of the two triangles using the Moller Trombore test.
///
/// @param lPos The vec3 for a point on the ray to be matched.
/// @param lDir The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.
/// @param offset A vec3 which gives the position of this element relative to its
/// containing object (since elements generally have relative positions, this is needed
/// to compute absolute position matches).

bool BuildingRect::matchRayToElement(vec3& lPos, vec3& lDir, float& lambda, vec3 offset)
{
  vec3      triangle[3];
  
  // First triangle
  glm_vec3_add(relativePos, offset, triangle[0]);
  glm_vec3_add(triangle[0], sides[0], triangle[1]);  
  glm_vec3_add(triangle[1], sides[1], triangle[2]);
  if(mollerTrumbore(triangle, lPos, lDir, lambda))
    return true;

  // Second triangle - going the other way round the square
  glm_vec3_add(triangle[0], sides[1], triangle[1]);  
  glm_vec3_add(triangle[1], sides[0], triangle[2]);
  if(mollerTrumbore(triangle, lPos, lDir, lambda))
    return true;

  return false;
}


// =======================================================================================
/// @brief Provide an HTML summary title/table for this BuildingRect.
/// @returns true if the information was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool  BuildingRect::httPrintTableSummary(HttpDebug* serv, char* ourName)
{
  httPrintf("<h4>%s</h4><center>\n", ourName);
  unless(serv->startTable(ourName))
    return false;

  httPrintf("<tr><th>Parameter Name</td><th>Value</th></tr>");
  httPrintf("<tr><td>Relative Position</td><td>%.2f, %.2f, %.2f</td></tr>", 
                                          relativePos[0], relativePos[1], relativePos[2]);
  httPrintf("<tr><td>Side Vector 0</td><td>%.2f, %.2f, %.2f</td></tr>", 
                                          sides[0][0], sides[0][1], sides[0][2]);
  httPrintf("<tr><td>Side Vector 1</td><td>%.2f, %.2f, %.2f</td></tr>", 
                                          sides[1][0], sides[1][1], sides[1][2]);
  return true;
}


// =======================================================================================
