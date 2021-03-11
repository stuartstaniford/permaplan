
// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for rendering a simple straight arrow.

#include <cstdio>
#include <err.h>
#include "Cylinder.h"


// =======================================================================================
// Constructors.

Cylinder::Cylinder(vec3 root, vec3 dir, float R, unsigned S):
                                                  radius(R),
                                                  sides(S)
{
  glm_vec3_copy(root, location);
  glm_vec3_copy(dir, axisDirection);
}


// =======================================================================================
// Destructor

Cylinder::~Cylinder(void)
{
}


// =======================================================================================
// Update a supplied bounding box with all our points, so that we are fully encompassed
// within it.  Returns whether or not any extension was required

bool Cylinder::updateBoundingBox(BoundingBox* box, float altitude)
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
    point[0] = location[0] + cosAng*f1[0] + sinAng*f2[0];
    point[2] = location[1] + cosAng*f1[1] + sinAng*f2[1];
    point[2] = location[2] + cosAng*f1[2] + sinAng*f2[2] + altitude;
    if(box->extends(point))
      retVal = true;
    
    // top of shaft
    point[0] += axisDirection[0];
    point[2] += axisDirection[1];
    point[2] += axisDirection[2];
    if(box->extends(point))
      retVal = true;
   }
  
  return retVal;
}


// =======================================================================================
//XX Stub definition needs to be implemented

bool Cylinder::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
//XX Stub definition needs to be implemented

bool Cylinder::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
//XX Stub definition needs to be implemented
int Cylinder::getNextIndex(bool resetToFirst)
{
  return -1;
}

// =======================================================================================
// Utility function to get two vectors perpendicular to a specified one.
// Eg if sides == 8 the cross section of the cylinder which points in the dir
// direction looks like
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

void getCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius)
{
  vec3 f0; // starting place, will be z-axis unless dir is parallel when we use x-axis.

  if(dir[0] < EPSILON && dir[1] < EPSILON)
   {
    f0[0] = 1.0f;
    f0[1] = f0[2] = 0.0f;
   }
  else
   {
    f0[0] = f0[1] = 0.0f;
    f0[2] = 1.0f;
   }

  glm_vec3_cross(f0, dir, f1);
  glm_vec3_cross(dir, f1, f2);
  glm_vec3_scale_as(f1, radius, f1);
  glm_vec3_scale_as(f2, radius, f2);
}


// =======================================================================================
// This is where the actual octahedron geometry is defined - we render it into a buffer
// on request

bool Cylinder::bufferGeometry(TriangleBuffer* T, float altitude, unsigned color)
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
    
    // base of shaft of the cylinder on this particular radial slice
    x = location[0] + cosAng*f1[0] + sinAng*f2[0];
    y = location[1] + cosAng*f1[1] + sinAng*f2[1];
    z = location[2] + cosAng*f1[2] + sinAng*f2[2] + altitude;
    vertices[2*i].set(x, y, z, ((color>>24)&0x000000ff)/256.0,
                      ((color>>16)&0x000000ff)/256.0, ((color>>8)&0x000000ff)/256.0);

    // top of shaft
    x += axisDirection[0];
    y += axisDirection[1];
    z += axisDirection[2];
    vertices[2*i+1].set(x, y, z, ((color>>24)&0x000000ff)/256.0,
                        ((color>>16)&0x000000ff)/256.0, ((color>>8)&0x000000ff)/256.0);
   }
  
  // Done with vertices, now set up the indices.  As usual, we need triangles
  // to be clockwise looking from outside the cylinder, because of OpenGL faceculling.
  
  int iPlus;
  for(int i=0; i<sides; i++)
   {
    iPlus = (i+1)%sides;
    indices[6*i]    = vOffset + 2*i;            // base of this radius
    indices[6*i+1]  = vOffset + 2*i + 1;        // top of shaft at this radius
    indices[6*i+2]  = vOffset + 2*iPlus;        // base of shaft at next radius
    indices[6*i+3]  = indices[6*i+2];           // base of shaft at next radius
    indices[6*i+4]  = indices[6*i+1];           // top of shaft at this radius
    indices[6*i+5]  = vOffset + 2*iPlus + 1;    // top of shaft at next radius
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
// Increase the length of the cylinder by a length increment.  Note we don't do anything
// about rebuffering - caller must track that.

void Cylinder::lengthen(float increment)
{
  float lenNow = glm_vec3_norm(axisDirection);
  glm_vec3_scale(axisDirection, (lenNow+increment)/lenNow, axisDirection);
}


// =======================================================================================
// Set the length of the cylinder to a supplied amount.  Note we don't do anything
// about rebuffering - caller must track that.

void Cylinder::setLength(float length)
{
  float lenNow = glm_vec3_norm(axisDirection);
  glm_vec3_scale(axisDirection, length/lenNow, axisDirection);
}


// =======================================================================================
// Figure out whether a ray intersects the cylinder or not
// https://en.wikipedia.org/wiki/Skew_lines#Distance

bool Cylinder::matchRay(vec3& position, vec3& direction, float& lambda)
{
#ifndef LOG_TREE_MATCH_RAY
  vec3 joinLine, originDiff;
#endif
  
  glm_vec3_crossn(direction, axisDirection, joinLine);
  glm_vec3_sub(position, location, originDiff);
  float dist = fabs(glm_vec3_dot(joinLine, originDiff));
#ifdef LOG_TREE_MATCH_RAY
  lastRayMatch = dist;
#endif

  if(dist <= radius)
   {
    lambda = NAN;  //XX haven't calculated this
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
