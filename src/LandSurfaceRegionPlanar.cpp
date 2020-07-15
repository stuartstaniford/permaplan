// Copyright Staniford Systems.  All Rights Reserved.  July 2020 -
// Class for storing and rendering a section of the landsurface mesh for some
// particular region in the quadtree (generally a leaf).  This class defines
// a simple planar version of the interface, with the plane defined by the
// constructor variables pos and norm - plane passes through pos and is perpendicular
// to norm.

#include <err.h>
#include "LandSurfaceRegionPlanar.h"

/* Arrangement of kids is as follows:
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
// Utility function that computes the height of a plane at some particular location

inline float planeHeight(vec3 plane, float x, float y)
{
  return x*plane[0] + y*plane[1] + plane[2];
}

// =======================================================================================
// Constructors.

LandSurfaceRegionPlanar::LandSurfaceRegionPlanar(float x, float y, float width, float height,
                           vec3 plane):LandSurfaceRegion(x,y,width,height)
{
  heights[0] = planeHeight(plane, x,          y);
  heights[1] = planeHeight(plane, x + width,  y);
  heights[2] = planeHeight(plane, x,          y + height);
  heights[3] = planeHeight(plane, x + width,  y + height);

  updateBoundingBox();
}


// =======================================================================================
// Destructor

LandSurfaceRegionPlanar::~LandSurfaceRegionPlanar(void)
{
}


// =======================================================================================
// Buffer our two triangles - we put the vertices in the same order as quadtree kids (see
// diagram up top).

bool LandSurfaceRegionPlanar::bufferGeometry(TriangleBuffer* T)
{
  VertexBufElement* vertices;
  unsigned* indices;
  unsigned vOffset, iOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, iOffset, 4u, 6u))
    return false;
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
  vertices[0].set(xyPos[0], xyPos[1], heights[0]); //lower left
  vertices[1].set(xyPos[0] + extent[0], xyPos[1], heights[1]); //lower right
  vertices[2].set(xyPos[0], xyPos[1] + extent[1], heights[2]);  //upper left
  vertices[3].set(xyPos[0] + extent[0], xyPos[1] + extent[1], heights[3]); //upper right
  
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
// How much space we need in a triangle buffer

void LandSurfaceRegionPlanar::triangleBufferSize(unsigned& vCount, unsigned& iCount)
{
  vCount = 4u;
  iCount = 6u;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void LandSurfaceRegionPlanar::draw(void)
{
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool LandSurfaceRegionPlanar::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(!box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
// Stub not done.
                                                                
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
// Stub definition this should be overwritten by implementing subclasses

bool LandSurfaceRegionPlanar::diagnosticHTML(HttpDebug* serv)
{
/*
  serv->addResponseData("<tr><td>HeightMarker</td>");
  serv->respPtr += sprintf(serv->respPtr,
                           "<td><b>Location:</b> (%.1f, %.1f, %.1f)</td></tr>\n",
                           location[0], location[1], location[2]);
*/
 return true;
}


// =======================================================================================
