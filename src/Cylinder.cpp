
// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for rendering a cylinder

#include <cstdio>
#include <err.h>
#include "Cylinder.h"


// =======================================================================================
// Constructors.

Cylinder::Cylinder(vec3 root, vec3 dir, float R, unsigned S):
                                                  AxialElement(root, dir, S),
                                                  radius(R)
{
}


// =======================================================================================
// Destructor

Cylinder::~Cylinder(void)
{
}


// =======================================================================================
// Update a supplied bounding box with all our points, so that we are fully encompassed
// within it.  Returns whether or not any extension was required

bool Cylinder::updateBoundingBox(BoundingBox* box, vec3 offset)
{
  bool  retVal        = false;
  float angleRadians  = 2.0f*M_PI/sides;
  float ang, cosAng, sinAng;
  vec3 point;

  getCrossVectors(axisDirection, f1, f2, radius);

  for(int i=0; i<sides; i++)
   {
    ang = i*angleRadians;
    cosAng = cosf(ang);
    sinAng = sinf(ang);
    
    // base of shaft of the cylinder on this particular radial slice
    point[0] = location[0] + cosAng*f1[0] + sinAng*f2[0] + offset[0];
    point[1] = location[1] + cosAng*f1[1] + sinAng*f2[1] + offset[1];
    point[2] = location[2] + cosAng*f1[2] + sinAng*f2[2] + offset[2];
    if(box->extends(point))
      retVal = true;
    
    // top of shaft
    point[0] += axisDirection[0];
    point[1] += axisDirection[1];
    point[2] += axisDirection[2];
    if(box->extends(point))
      retVal = true;
   }
  
  return retVal;
}


// =======================================================================================
// This is where the actual geometry is defined - we render it into a buffer on request

bool Cylinder::bufferGeometry(TriangleBuffer* T, vec3 offset)
{
  float     angleRadians  = 2.0f*M_PI/sides;
  Vertex*   vertices;
  unsigned* indices;
  unsigned  vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset,
                         sides*2, // vertices in the cylinder
                         sides*6))  // indices of vertices of triangles
    return false;

  getCrossVectors(axisDirection, f1, f2, radius);

  // Now that we've done some initial setup, we can compute all the vertices.
  float ang, cosAng, sinAng, x, y, z;
  for(int i=0; i<sides; i++)
   {
    ang = i*angleRadians;
    cosAng = cosf(ang);
    sinAng = sinf(ang);
    vec3 norm = {cosAng*f1[0] + sinAng*f2[0],
                    cosAng*f1[1] + sinAng*f2[1],
                    cosAng*f1[2] + sinAng*f2[2]};
    
    // base of shaft of the cylinder on this particular radial slice
    x = location[0] + norm[0] + offset[0];
    y = location[1] + norm[1] + offset[1];
    z = location[2] + norm[2] + offset[2];
    vertices[2*i].setPosition(x, y, z);
    vertices[2*i].setColor(color);
    vertices[2*i].setNormal(norm); // to be normalized in gpu

    // top of shaft
    x += axisDirection[0];
    y += axisDirection[1];
    z += axisDirection[2];
    vertices[2*i+1].setPosition(x, y, z);
    vertices[2*i+1].setColor(color);
    vertices[2*i+1].setNormal(norm);
   }
  
  // Done with vertices, now set up the indices.  As usual, we need triangles to be
  // counter-clockwise looking from outside the cylinder, because of OpenGL faceculling.
  int iPlus;
  for(int i=0; i<sides; i++)
   {
    iPlus = (i+1)%sides;
    indices[6*i]    = vOffset + 2*i;            // base of this radius
    indices[6*i+1]  = vOffset + 2*iPlus;        // base of shaft at next radius
    indices[6*i+2]  = vOffset + 2*i + 1;        // top of shaft at this radius
    indices[6*i+3]  = indices[6*i+1];           // base of shaft at next radius
    indices[6*i+4]  = vOffset + 2*iPlus + 1;    // top of shaft at next radius
    indices[6*i+5]  = indices[6*i+2];           // top of shaft at this radius
   }

  return true;
}


// =======================================================================================
// How much space we need in a TriangleBuffer

void Cylinder::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = sides*2;
  iCount = sides*6;
}


// =======================================================================================
// Figure out whether a ray intersects the cylinder or not
// https://en.wikipedia.org/wiki/Skew_lines#Distance

bool Cylinder::matchRay(vec3& position, vec3& direction, float& lambda, vec3 offset)
{
#ifndef LOG_TREE_MATCH_RAY
  vec3 joinLine, originDiff;
#endif
  vec3 relativePos;

  glm_vec3_sub(position, offset, relativePos);
  glm_vec3_crossn(direction, axisDirection, joinLine);
  glm_vec3_sub(relativePos, location, originDiff);
  float dist = fabs(glm_vec3_dot(joinLine, originDiff));
#ifdef LOG_TREE_MATCH_RAY
  lastRayMatch = dist;
#endif

  if(dist <= radius)
    return matchRayBruteForce(position, direction, lambda, offset);
  else
    return false;
}


// =======================================================================================
// This matches every triangle to be certain whether the ray hits or not
// XX might return a hit on far side of cylinder, instead of nearest.

bool Cylinder::matchRayBruteForce(vec3& position, vec3& direction, float& lambda, vec3 offset)
{
  float     angleRadians  = 2.0f*M_PI/sides;
  vec3      triangle[3];
  
  getCrossVectors(axisDirection, f1, f2, radius);

  // Now that we've done some initial setup, we can compute all the vertices.
  float ang, cosAng, sinAng;
  for(int i=0; i<sides; i++)
   {
    ang = i*angleRadians;
    cosAng = cosf(ang);
    sinAng = sinf(ang);
    vec3 norm = {cosAng*f1[0] + sinAng*f2[0],
                    cosAng*f1[1] + sinAng*f2[1],
                    cosAng*f1[2] + sinAng*f2[2]};
    
    // base of shaft of the cylinder on this particular radial slice
    triangle[0][0] = location[0] + norm[0] + offset[0];
    triangle[0][1] = location[1] + norm[1] + offset[1];
    triangle[0][2] = location[2] + norm[2] + offset[2];

    // top of shaft
    triangle[1][0] = triangle[0][0] + axisDirection[0];
    triangle[1][1] = triangle[0][1] + axisDirection[1];
    triangle[1][2] = triangle[0][2] + axisDirection[2];

    // Now complete the triangle with base of next radial slice
    ang = ((i+1)%sides)*angleRadians;
    cosAng = cosf(ang);
    sinAng = sinf(ang);
    norm[0] = cosAng*f1[0] + sinAng*f2[0];
    norm[1] = cosAng*f1[1] + sinAng*f2[1];
    norm[2] = cosAng*f1[2] + sinAng*f2[2];

    triangle[2][0] = location[0] + norm[0] + offset[0];
    triangle[2][1] = location[1] + norm[1] + offset[1];
    triangle[2][2] = location[2] + norm[2] + offset[2];

    // test the triangle
    if(mollerTrumbore(triangle, position, direction, lambda))
      return true;

    // top of shaft
    triangle[2][0] += axisDirection[0];
    triangle[2][1] += axisDirection[1];
    triangle[2][2] += axisDirection[2];
  
    // ok test again
    if(mollerTrumbore(triangle, position, direction, lambda))
      return true;
   }
  
  return false;
}


// =======================================================================================
// Function to print out in JSON format.

#define bufprintf(...) if((buf += snprintf(buf, end-buf,  __VA_ARGS__)) >= end) {return -1;}

int Cylinder::printOPSF(char*& buf, unsigned bufSize)
{
  char* end = buf + bufSize;

  return (bufSize - (int)(end-buf));
}


// =======================================================================================
// Tell callers our name at runtime.

const char* Cylinder::objectName(void)
{
  static char* name = (char*)"Cylinder";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular HeightMarker.

bool Cylinder::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("<tr><td>Cylinder</td>");
  httPrintf("<td><b>Location:</b> (%.1f, %.1f, %.1f)<br>",
                                            location[0], location[1], location[2]);
  httPrintf("<b>Radius:</b> %.1f<br>", radius);
  httPrintf("<b>Sides:</b> %u<br>", sides);
  httPrintf("axisDirection:</b> (%.1f, %.1f, %.1f)</td></tr>\n" ,
                                            axisDirection[0], axisDirection[1], axisDirection[2]);
  return true;
}


// =======================================================================================
