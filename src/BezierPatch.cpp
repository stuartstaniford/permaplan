// Copyright Staniford Systems.  All Rights Reserved.  Aug 2020 -
// A set of control points to manage a cubic bezier patch on some particular
// node in the Quadtree

#include <err.h>
#include "BezierPatch.h"


// =======================================================================================
// Constructors.

BezierPatch::BezierPatch(float x, float y, float width, float height,
                        float s, float t, float sWidth, float tHeight, unsigned gridPoints):
            LandSurfaceRegion(x, y, width, height, s, t, sWidth, tHeight), gridN(gridPoints),
            fitGoodEnough(false), fitPointUVVals()
{
}

BezierPatch::BezierPatch(Quadtree* qtree, unsigned gridPoints):
LandSurfaceRegion(qtree->bbox.lower[0], qtree->bbox.lower[1],
                  qtree->bbox.upper[0] - qtree->bbox.lower[0],
                  qtree->bbox.upper[1] - qtree->bbox.lower[1],
                  0.0f, 0.0f, 1.0f, 1.0f), gridN(gridPoints)
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

void BezierPatch::surfacePoint(float u, float v, vec3 result)
{
  glm_vec3_zero(result);
  
  float upow[4], vpow[4], u1minpow[4], v1minpow[4];
  upow[0] = vpow[0] = u1minpow[0] = v1minpow[0] = 1.0f;
  for(int i=1; i<4; i++)
   {
    upow[i]     = u*upow[i-1];
    vpow[i]     = v*vpow[i-1];
    u1minpow[i] = (1.0f - u)*u1minpow[i-1];
    v1minpow[i] = (1.0f - v)*v1minpow[i-1];
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
// For anyone who needs to know how much space we would take up in a triangleBuffer.

void BezierPatch::triangleBufferSizes(unsigned& vertexCount, unsigned& indexCount)
{
  vertexCount = (gridN+1)*(gridN+1);
  indexCount = 6*gridN*gridN;
}

// =======================================================================================
// Buffer our triangles.

bool BezierPatch::bufferGeometry(TriangleBuffer* T)
{
  // Establish space needs and obtain buffers
  VertexBufElement* vertices;
  unsigned* indices;
  unsigned vOffset, iOffset;
  unsigned vCount, iCount;
  triangleBufferSizes(vCount, iCount);
  unless(T->requestSpace(&vertices, &indices, vOffset, iOffset, vCount, iCount))
    return false;

  //Figure out the vertices
  float spacing = 1.0f/(float)gridN;
  unsigned i,j;
  float u,v;
  for (i=0, u=0.0f; i<=gridN; i++, u+=spacing)
    for (j=0, v=0.0f; j<=gridN; j++, v+=spacing)
     {
      VertexBufElement* bufEl = vertices + i*(gridN + 1) + j;
      surfacePoint(u, v, bufEl->pos);
      bufEl->tex[0] = stPos[0] + stExtent[0]*spacing*(float)i;
      bufEl->tex[1] = stPos[1] + stExtent[1]*spacing*(float)j;
      bufEl->accent = 0.0f;
      //printf("u,v: %.3f, %.3f\t", u, v);
      //bufEl->fprint(stdout);
     }

  for (i=0; i<gridN; i++)
    for (j=0; j<gridN; j++)
     {
      unsigned base = 6*(i*gridN + j);
      indices[base] = i*(gridN + 1) + j;
      indices[base + 1] = (i+1)*(gridN + 1) + j;
      indices[base + 2] = i*(gridN + 1) + j + 1;
      indices[base + 3] = indices[base + 1];
      indices[base + 4] = (i+1)*(gridN + 1) + j + 1;
      indices[base + 5] = indices[base + 2];
     }

  //T->fprint(stderr);
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
// This creates a random Bezier patch (which can later be improved.

#define setControlPoints(i,j,x,y,z) controlPoints[i][j][0]=x;controlPoints[i][j][1]=y;controlPoints[i][j][2]=z
#define randHeight arc4random()/(float)UINT32_MAX*100.0f-50.0f

void BezierPatch::randomFit(std::vector<float*>& locations)
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

  fitGoodEnough = false;
}


// =======================================================================================
// Make an incremental improvement in the fit of the patch to the known locations.

void BezierPatch::improveFit(std::vector<float*>& locations)
{
  if(fitGoodEnough)
    return;
  
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular chunk of land surface.  Since BezierPatch is a bit complex
// we have a little mini-table of all our control points inside the cell.

bool BezierPatch::diagnosticHTML(HttpDebug* serv)
{
  serv->addResponseData("<tr><td>BezierPatch</td><td>");
  serv->addResponseData("<table cellpadding=1 border=1><tr><th>i</th><th>j</th><th>X</th><th>Y</th><th>Z</th></tr>");
  for(int i=0; i<4; i++)
    for(int j=0; j<4; j++)
  serv->respPtr += sprintf(serv->respPtr,
              "<tr><td>%d</td><td>%d</td><td>%.1f</td><td>%.1f</td><td>%.1f</td></tr>",
              i, j, controlPoints[i][j][0], controlPoints[i][j][1], controlPoints[i][j][2]);
  serv->addResponseData("</table></td></tr>\n");

  return true;
}


// =======================================================================================
