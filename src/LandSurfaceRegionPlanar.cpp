// Copyright Staniford Systems.  All Rights Reserved.  July 2020 -
// Class for storing and rendering a section of the landsurface mesh for some
// particular region in the quadtree (generally a leaf).  This class defines
// a simple planar version of the interface, with the plane defined by the
// constructor variables pos and norm - plane passes through pos and is perpendicular
// to norm.

#include "LandSurfaceRegionPlanar.h"
#include "HttpDebug.h"
#include <err.h>

/* For reference: the arrangement of kids in a quadtree node is as follows:
 ---------------
 |      |      |
 | k[2] | k[3] |
 |      |      |
 ---------------
 |      |      |
 | k[0] | k[1] |
 |      |      |
 ---------------      */


// =======================================================================================
/// @brief Utility function that computes the height of a plane at some particular 
/// location

inline float planeHeight(vec3 plane, float x, float y)
{
  return x*plane[0] + y*plane[1] + plane[2];
}


// =======================================================================================
/// @brief Constructor.
///
/// @param x x-coordinate of the south-west corner of the covered region
/// @param y y-coordinate of the south-west corner of the covered region
/// @param width width of the covered region
/// @param height height of the covered region
/// @param s s texture-coordinate of the south-west corner of the covered region
/// @param t t texture-coordinate of the south-west corner of the covered region
/// @param sWidth width in texture space of the covered region
/// @param tHeight height in texture space of the covered region
/// @param plane A vec3 that specifies the plane we are to implemnt.  The zeroth
/// component is the coefficient of x, the one component is the coefficient of y, and
/// the 2 component is the height of the plane when x and y are both zero.

LandSurfaceRegionPlanar::LandSurfaceRegionPlanar(float x, float y, float width, float height,
                          float s, float t, float sWidth, float tHeight, vec3 plane):
                          LandSurfaceRegion(x, y, width, height, s, t, sWidth, tHeight)
{
  resetPlane(plane);
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
}


// =======================================================================================
/// @brief Destructor

LandSurfaceRegionPlanar::~LandSurfaceRegionPlanar(void)
{
}


// =======================================================================================
/// @brief Set up our height variables based on our plane
/// @param plane.  A vec3 that specifies the plane we are to implemnt.  The zeroth
/// component is the coefficient of x, the one component is the coefficient of y, and
/// the 2 component is the height of the plane when x and y are both zero.

void LandSurfaceRegionPlanar::resetPlane(vec3 plane)
{
  heights[0] = planeHeight(plane, xyPos[0],             xyPos[1]);
  heights[1] = planeHeight(plane, xyPos[0] + extent[0], xyPos[1]);
  heights[2] = planeHeight(plane, xyPos[0],             xyPos[1] + extent[1]);
  heights[3] = planeHeight(plane, xyPos[0] + extent[0], xyPos[1] + extent[1]);
  updateBoundingBox();
}


// =======================================================================================
/// @brief Buffer our two triangles - we put the vertices in the same order as 
/// quadtree kids (see diagram up top).
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).

bool LandSurfaceRegionPlanar::bufferGeometryOfObject(TriangleBuffer* T)
{
  Vertex*   vertices;
  unsigned* indices;
  unsigned  vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, 4u, 6u))
   {
    LogTriangleBufferErrs("Box TriangleBuffer request for %u,%u failed at %u.\n",
                                                    4u, 6u, vOffset);
    return false;
   }
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
  
  //lower left
  vertices[0].setPosition(xyPos[0], xyPos[1], heights[0]);
  vertices[0].setTexCoords(stPos[0], stPos[1]); 
  vertices[0].setObjectId(objIndex);

  //lower right
  vertices[1].setPosition(xyPos[0] + extent[0], xyPos[1], heights[1]);
  vertices[1].setTexCoords(stPos[0] + stExtent[0], stPos[1]); 
  vertices[1].setObjectId(objIndex);
  
  //upper left
  vertices[2].setPosition(xyPos[0], xyPos[1] + extent[1], heights[2]);
  vertices[2].setTexCoords(stPos[0], stPos[1] + stExtent[1]);  
  vertices[2].setObjectId(objIndex);
  
  //upper right
  vertices[3].setPosition(xyPos[0] + extent[0], xyPos[1] + extent[1], heights[3]);
  vertices[3].setTexCoords(stPos[0] + stExtent[0], stPos[1] + stExtent[1]); 
  vertices[3].setObjectId(objIndex);
  
  // Lower left triangle
  indices[0] = vOffset;
  indices[1] = vOffset + 1u;
  indices[2] = vOffset + 2u;
  
  // Upper right triangle
  indices[3] = vOffset + 1u;
  indices[4] = vOffset + 3u;
  indices[5] = vOffset + 2u;

  return true;
}


// =======================================================================================
/// @brief How much space we need in a TriangleBuffer
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.

void LandSurfaceRegionPlanar::triangleBufferSize(unsigned& vCount, unsigned& iCount)
{
  vCount = 4u;
  iCount = 6u;
  LogTriangleBufEstimates("LandSurfaceRegionPlanar TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
/// @brief Decide if a ray touches us.  
/// 
/// Function to decide whether a given line touches the object or not.
/// @returns True if the ray touches us, otherwise false.
/// @param position The vec3 for a point on the ray to be matched.
/// @param direction The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.
/// @todo This is not fully implemented and only checks bounding box match.

bool LandSurfaceRegionPlanar::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(!box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
/// @brief Recompute our bounding box when a caller needs it done.
                                                                
void LandSurfaceRegionPlanar::updateBoundingBox(void)
{
  float lowZ  = HUGE_VALF;
  float highZ = -HUGE_VALF;
  
  for(int i = 0; i<4; i++)
   {
    if(heights[i] < lowZ)
      lowZ = heights[i];
    if(heights[i] > highZ)
      highZ = heights[i];
   }
  if(!box)
    box = new BoundingBox(xyPos[0], xyPos[1], lowZ,
                          xyPos[0] + extent[0], xyPos[1] + extent[1], highZ);
  else
    box->reset(xyPos[0], xyPos[1], lowZ,
               xyPos[0] + extent[0], xyPos[1] + extent[1], highZ);
  return;
}
                                                                

// =======================================================================================
/// @brief Stub not done.

void  LandSurfaceRegionPlanar::fit(std::vector<float*>& locations)
{
  
}


// =======================================================================================
/// @brief Tell callers our name at runtime.
/// @returns A const pointer to a C-string with the name.

const char* LandSurfaceRegionPlanar::objectName(void)
{
  static char* name = (char*)"Land Surface Plane";
  return name;
}


// =======================================================================================
/// @brief Provides access to the diagnostic HTTP server for the diagnosticHTML page of 
/// this LandSurfaceRegionPlanar via it's objIndex.
/// 
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool LandSurfaceRegionPlanar::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("<tr><td>LandSurfaceRegionPlanar</td><td>");
  httPrintf("<b>XY Position:</b> (%.1f, %.1f)<br>", xyPos[0], xyPos[1]);
  httPrintf("<b>Extent:</b> (%.1f, %.1f)<br>", extent[0], extent[1]);
  httPrintf("<b>Heights:</b> (%.1f, %.1f, %.1f, %.1f)<br>",
                                              heights[0], heights[1], heights[2], heights[3]);
  httPrintf( "<b>ST Position:</b> (%.4f, %.4f)<br>", stPos[0], stPos[1]);
  httPrintf( "<b>ST Extent:</b> (%.4f, %.4f)<br>", stExtent[0], stExtent[1]);
  httPrintf("</td></tr>\n");

 return true;
}


// =======================================================================================
