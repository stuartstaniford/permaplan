
// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Class for rendering a simple straight arrow.

#include <cstdio>
#include <err.h>
#include "Cylinder.h"
#include "Arrow.h"


// =======================================================================================
// Constructors.

Arrow::Arrow(vec3 root, vec3 dir):
                  VisualObject(true)
{
  incrementArrowMemory(sizeof(Arrow));
  glm_vec3_copy(root, location);
  glm_vec3_copy(dir, direction);
  updateBoundingBox();
}


// =======================================================================================
// Destructor

Arrow::~Arrow(void)
{
}


// =======================================================================================
//XX Stub definition needs to be implemented

bool Arrow::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
//XX Stub definition needs to be implemented
bool Arrow::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
//XX Stub definition needs to be implemented
int Arrow::getNextIndex(bool resetToFirst)
{
  return -1;
}


// =======================================================================================
// This is where the actual octahedron geometry is defined - we render it into a buffer
// on request

float arrowBody = (1.0f - ARROW_HEAD);
float angleRadians  = 2.0f*M_PI/ARROW_SIDES;

bool Arrow::bufferGeometry(TriangleBuffer* T)
{
  Vertex*   vertices;
  unsigned* indices;
  unsigned  vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset,
                         ARROW_SIDES*3+1, // vertices in the arrow
                         ARROW_SIDES*9))  // indices of vertices of triangles
    return false;

  float arrowRadius   = glm_vec3_norm(direction)*ARROW_RADIUS;

  vec3 f1, f2; // Set up axes perpendicular to direction, which will be called f1 and f2.
  getCrossVectors(direction, f1, f2);
  glm_vec3_scale_as(f1, arrowRadius, f1);
  glm_vec3_scale_as(f2, arrowRadius, f2);

  // Now that we've done some initial setup, we can compute all the vertices.

  float ang, cosAng, sinAng, x, y, z;
  for(unsigned i=0; i<ARROW_SIDES; i++)
   {
    ang = i*angleRadians;
    cosAng = cos(ang);
    sinAng = sin(ang);
    
    // base of shaft of the arrow on this particular radial slice
    x = location[0] + cosAng*f1[0] + sinAng*f2[0];
    y = location[1] + cosAng*f1[1] + sinAng*f2[1];
    z = location[2] + cosAng*f1[2] + sinAng*f2[2];
    vertices[3*i].set(x, y, z);

    // top of shaft
    x += arrowBody*direction[0];
    y += arrowBody*direction[1];
    z += arrowBody*direction[2];
    vertices[3*i+1].set(x, y, z);
    
    // base of arrow head
    x += ARROW_HEAD_WIDTH*(cosAng*f1[0] + sinAng*f2[0]);
    y += ARROW_HEAD_WIDTH*(cosAng*f1[1] + sinAng*f2[1]);
    z += ARROW_HEAD_WIDTH*(cosAng*f1[2] + sinAng*f2[2]);
    vertices[3*i+2].set(x, y, z);
   }
 
  // tip of the arrow
  vertices[3*ARROW_SIDES].set(location[0]+direction[0], location[1]+direction[1],
                              location[2]+direction[2]);
 
  if(useNoTexColor)
    for(int i=0; i<ARROW_SIDES*3+1; i++)
      vertices[i].setNoTexColor(noTexColor);

  // Done with vertices, now set up the indices.  As usual, we need triangles
  // to be clockwise looking from outside the arrow, because of OpenGL faceculling.
  
  int iPlus;
  for(unsigned i=0; i<ARROW_SIDES; i++)
   {
    iPlus = (i+1)%ARROW_SIDES;
    indices[9*i]    = vOffset + 3*i;            // base of this radius
    indices[9*i+1]  = vOffset + 3*i + 1u;       // top of shaft at this radius
    indices[9*i+2]  = vOffset + 3*iPlus;        // base of shaft at next radius
    indices[9*i+3]  = indices[9*i+2];           // base of shaft at next radius
    indices[9*i+4]  = indices[9*i+1];           // top of shaft at this radius
    indices[9*i+5]  = vOffset + 3*iPlus + 1u;   // top of shaft at next radius
    indices[9*i+6]  = vOffset + 3*i + 2u;       // base of head at this radius
    indices[9*i+7]  = vOffset + 3*ARROW_SIDES;  // tip of the arrow
    indices[9*i+8]  = vOffset + 3*iPlus + 2u;   // base of head at next radius
   }

  return true;
}


// =======================================================================================
// How much space we need in a TriangleBuffer

void Arrow::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = ARROW_SIDES*3+1;
  iCount = ARROW_SIDES*9;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool Arrow::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(!box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
// Bounding box is aligned with the square of the octahedron (which is aligned in the x-y
// plane).  Height markers are currently immutable once created.

//XX arrow bounding box not implemented yet.

void Arrow::updateBoundingBox(void)
{
/*
   if(!box)
    {
    box = new BoundingBox(location[0] - heightMarkerSize, location[1] - heightMarkerSize,
                      location[2], location[0] + heightMarkerSize,
                      location[1] + heightMarkerSize, location[2] + 2.0f*heightMarkerHeight);
    incrementArrowMemory(sizeof(BoundingBox));
    }
 */
 return;
}
                                                                

// =======================================================================================
// Tell callers our name at runtime.

const char* Arrow::objectName(void)
{
  static char* name = (char*)"Arrow";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular HeightMarker.

bool Arrow::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("<tr><td>Arrow</td>");
  httPrintf("<td><b>Location:</b> (%.1f, %.1f, %.1f)<br>" ,
                                  location[0], location[1], location[2]);
  httPrintf("Direction:</b> (%.1f, %.1f, %.1f)</td></tr>\n" ,
                           direction[0], direction[1], direction[2]);
  return true;
}


// =======================================================================================
