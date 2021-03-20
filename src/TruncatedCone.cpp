
// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Class for rendering a section of a cone cut off at right angles at both ends.
// Used for rendering sections of tree-trunks and the like.

#include <cstdio>
#include <err.h>
#include "TruncatedCone.h"


// =======================================================================================
// Constructors.

TruncatedCone::TruncatedCone(vec3 root, vec3 dir, float r, float R, unsigned S):
                                                  AxialElement(root, dir, R, S),
                                                  smallRadius(r),
                                                  conePoints{{1.0f, 0.0f}, {r/R, 1.0f}}
{
  NVecs       = 2u;
  vectorPath  = conePoints;
}


// =======================================================================================
// Destructor

TruncatedCone::~TruncatedCone(void)
{
}


// =======================================================================================
// Function to print out in JSON format.

#define bufprintf(...) if((buf += snprintf(buf, end-buf,  __VA_ARGS__)) >= end) {return -1;}

int TruncatedCone::printOPSF(char*& buf, unsigned bufSize)
{
  char* end = buf + bufSize;

  return (bufSize - (int)(end-buf));
}


// =======================================================================================
// Tell callers our name at runtime.

const char* TruncatedCone::objectName(void)
{
  static char* name = (char*)"TruncatedCone";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular HeightMarker.

bool TruncatedCone::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("<tr><td>TruncatedCone</td>");
  httPrintf("<td><b>Location:</b> (%.1f, %.1f, %.1f)<br>",
                                            location[0], location[1], location[2]);
  httPrintf("<b>Bottom (large) radius:</b> %.1f<br>", radius);
  httPrintf("<b>Top (small) radius:</b> %.1f<br>", smallRadius);
  httPrintf("<b>Sides:</b> %u<br>", sides);
  httPrintf("axisDirection:</b> (%.1f, %.1f, %.1f)</td></tr>\n" ,
                                            axisDirection[0], axisDirection[1], axisDirection[2]);
  return true;
}


// =======================================================================================
