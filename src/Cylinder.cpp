
// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for rendering a cylinder

#include <cstdio>
#include <err.h>
#include "Cylinder.h"


// =======================================================================================
// Constructors.

Cylinder::Cylinder(vec3 root, vec3 dir, float R, unsigned S):
                                              AxialElement(root, dir, R, S),
                                              cylinderPoints{{1.0f, 0.0f}, {1.0f, 1.0f}}
{
  NVecs       = 2u;
  vectorPath  = cylinderPoints;
}


// =======================================================================================
// Destructor

Cylinder::~Cylinder(void)
{
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
