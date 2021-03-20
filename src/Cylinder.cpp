
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
