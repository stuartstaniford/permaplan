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

BezierPatch::BezierPatch(Quadtree* qtree):
LandSurfaceRegion(qtree->bbox.lower[0], qtree->bbox.lower[1],
                  qtree->bbox.upper[0] - qtree->bbox.lower[0],
                  qtree->bbox.upper[1] - qtree->bbox.lower[1],
                  0.0f, 0.0f, 1.0f, 1.0f)
{
}


// =======================================================================================
// Destructor

BezierPatch::~BezierPatch(void)
{
}


// =======================================================================================
// Computes the height of the patch at some particular location in parametric
// space. For background, see:
// https://www.scratchapixel.com/lessons/advanced-rendering/bezier-curve-rendering-utah-teapot/bezier-surface

float bern[4] = {1.0f, 3.0f, 3.0f, 1.0f};

void BezierPatch::surfacePoint(vec3 result, float u, float v)
{
  glm_vec3_zero(result);
  
  float upow[4], vpow[4], u1minpow[4], v1minpow[4];
  upow[0] = vpow[0] = u1minpow[0] = v1minpow[0] = 1.0f;
  for(int i=1; i<4; i++)
   {
    upow[i]     = u*upow[i-1];
    vpow[i]     = v*vpow[i-1];
    u1minpow[i] = (u-1.0f)*u1minpow[i-1];
    v1minpow[i] = (v-1.0f)*v1minpow[i-1];
   }

  for(int i=0; i<4; i++)
    for(int j=0; j<4; j++)
     {
      float scale = bern[i]*upow[i]*u1minpow[3-i]*bern[j]*vpow[j]*v1minpow[3-j];
      vec3 scaledControlPoint;
      glm_vec3_scale(controlPoints[i][j], scale, scaledControlPoint);
      glm_vec3_add(result, scaledControlPoint, result);
     }
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
// Fit our patch to a vector of locations

// Right now this is a stub with hard-coded control points
#define setControlPoints(i,j,x,y,z) controlPoints[i][j][0]=x;controlPoints[i][j][1]=y;controlPoints[i][j][2]=z
#define randHeight arc4random()/(float)UINT32_MAX*100.0f-50.0f

void BezierPatch::fit(std::vector<float*>& locations)
{
  // Bottom row of control points
  setControlPoints(0, 0, xyPos[0],                    xyPos[1],                   randHeight);
  setControlPoints(1, 0, xyPos[0] + 0.33f*extent[0],  xyPos[1],                   randHeight);
  setControlPoints(2, 0, xyPos[0] + 0.66f*extent[0],  xyPos[1],                   randHeight);
  setControlPoints(3, 0, xyPos[0] + extent[0],        xyPos[1],                   randHeight);
  
  // Second row of control points
  setControlPoints(0, 1, xyPos[0],                    xyPos[1] + 0.33f*extent[1], randHeight);
  setControlPoints(1, 1, xyPos[0] + 0.33f*extent[0],  xyPos[1] + 0.33f*extent[1], randHeight);
  setControlPoints(2, 1, xyPos[0] + 0.66f*extent[0],  xyPos[1] + 0.33f*extent[1], randHeight);
  setControlPoints(3, 1, xyPos[0] + extent[0],        xyPos[1] + 0.33f*extent[1], randHeight);
  
  // Third row of control points
  setControlPoints(0, 2, xyPos[0],                    xyPos[1] + 0.66f*extent[1], randHeight);
  setControlPoints(1, 2, xyPos[0] + 0.33f*extent[0],  xyPos[1] + 0.66f*extent[1], randHeight);
  setControlPoints(2, 2, xyPos[0] + 0.66f*extent[0],  xyPos[1] + 0.66f*extent[1], randHeight);
  setControlPoints(3, 2, xyPos[0] + extent[0],        xyPos[1] + 0.66f*extent[1], randHeight);

  // Fourth row of control points
  setControlPoints(0, 3, xyPos[0],                    xyPos[1] + extent[1],       randHeight);
  setControlPoints(1, 3, xyPos[0] + 0.33f*extent[0],  xyPos[1] + extent[1],       randHeight);
  setControlPoints(2, 3, xyPos[0] + 0.66f*extent[0],  xyPos[1] + extent[1],       randHeight);
  setControlPoints(3, 3, xyPos[0] + extent[0],        xyPos[1] + extent[1],       randHeight);

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
