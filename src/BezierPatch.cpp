// Copyright Staniford Systems.  All Rights Reserved.  Aug 2020 -
// A set of control points to manage a cubic bezier patch on some particular
// node in the Quadtree

#include <err.h>
#include "BezierPatch.h"


// =======================================================================================
// Constructors.

BezierPatch::BezierPatch(float x, float y, float width, float height,
                                                 float s, float t, float sWidth, float tHeight):
LandSurfaceRegion(x, y, width, height, s, t, sWidth, tHeight)
{
}


// =======================================================================================
// Destructor

BezierPatch::~BezierPatch(void)
{
}


// =======================================================================================
// Computes the height of the patch at some particular location

float BezierPatch::surfaceHeight(float x, float y)
{
  return 0.0f;
}

// =======================================================================================
// Buffer our two triangles - we put the vertices in the same order as quadtree kids (see
// diagram up top).

bool BezierPatch::bufferGeometry(TriangleBuffer* T)
{
  /*
  VertexBufElement* vertices;
  unsigned* indices;
  unsigned vOffset, iOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, iOffset, 4u, 6u))
  return false;
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
  vertices[0].set(xyPos[0], xyPos[1], heights[0],
                  stPos[0], stPos[1]); //lower left
  vertices[1].set(xyPos[0] + extent[0], xyPos[1], heights[1],
                  stPos[0] + stExtent[0], stPos[1]); //lower right
  vertices[2].set(xyPos[0], xyPos[1] + extent[1], heights[2],
                  stPos[0], stPos[1] + stExtent[1]);  //upper left
  vertices[3].set(xyPos[0] + extent[0], xyPos[1] + extent[1], heights[3],
                  stPos[0] + stExtent[0], stPos[1] + stExtent[1]); //upper right
  
  // Lower left triangle
  indices[0] = vOffset;
  indices[1] = vOffset + 1u;
  indices[2] = vOffset + 2u;
  
  // Upper right triangle
  indices[3] = vOffset + 1u;
  indices[4] = vOffset + 3u;
  indices[5] = vOffset + 2u;
*/
  return true;
}


// =======================================================================================
// How much space we need in a triangle buffer

void BezierPatch::triangleBufferSize(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void BezierPatch::draw(void)
{
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool BezierPatch::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(!box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
// Recompute our bounding box when a caller needs it done.

void BezierPatch::updateBoundingBox(void)
{
  /*
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
*/
   return;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular chunk of land surface.

bool BezierPatch::diagnosticHTML(HttpDebug* serv)
{
  /*
  serv->addResponseData("<tr><td>LandSurfaceRegionPlanar</td><td>");
  serv->respPtr += sprintf(serv->respPtr, "<b>XY Position:</b> (%.1f, %.1f)<br>",
                           xyPos[0], xyPos[1]);
  serv->respPtr += sprintf(serv->respPtr, "<b>Extent:</b> (%.1f, %.1f)<br>",
                           extent[0], extent[1]);
  serv->respPtr += sprintf(serv->respPtr, "<b>Heights:</b> (%.1f, %.1f, %.1f, %.1f)<br>",
                           heights[0], heights[1], heights[2], heights[3]);
  serv->respPtr += sprintf(serv->respPtr, "<b>ST Position:</b> (%.4f, %.4f)<br>",
                           stPos[0], stPos[1]);
  serv->respPtr += sprintf(serv->respPtr, "<b>ST Extent:</b> (%.4f, %.4f)<br>",
                           stExtent[0], stExtent[1]);
  serv->addResponseData("</td></tr>\n");
*/
  return true;
}


// =======================================================================================
