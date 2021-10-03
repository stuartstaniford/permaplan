// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Class for rendering a section of a cone cut off at right angles at both ends.
// Used for rendering sections of tree-trunks and the like.

#include "TruncatedCone.h"
#include "HttpDebug.h"
#include <cstdio>
#include <err.h>


// =======================================================================================
/// @brief Constructor.
/// @param root A vector to the base of the axis of the cone.
/// @param dir A vector of the length and direction of the axis of the truncated cone.
/// @param r The (float) small radius at the truncated top of the cylinder.
/// @param R The (float) large radius at the base of the cylinder.
/// @param S An unsigned for the number of sides to use in approximating the cylinder.

TruncatedCone::TruncatedCone(vec3 root, vec3 dir, float r, float R, unsigned S):
                                                  AxialElement(root, dir, R, S),
                                                  smallRadius(r),
                                                  conePoints{{1.0f, 0.0f}, {r/R, 1.0f}}
{
  NVecs       = 2u;
  vectorPath  = conePoints;
}


// =======================================================================================
/// @brief Destructor

TruncatedCone::~TruncatedCone(void)
{
}


// =======================================================================================
/// @brief Function to print out in JSON format.
/// @todo not implemented.

int TruncatedCone::printOPSF(char*& buf, unsigned bufSize)
{
  char* end = buf + bufSize;

  return (bufSize - (int)(end-buf));
}


// =======================================================================================
/// @brief Tell callers our name at runtime.
/// @returns A const pointer to a C-string with the name.

const char* TruncatedCone::objectName(void)
{
  static char* name = (char*)"TruncatedCone";
  return name;
}


// =======================================================================================
/// @brief Report on our object to an HTTP page.
/// 
/// We assume we are part of a table of visual objects and we just contribute 
/// one row about this particular object.  This is the version called from Quadtree pages
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

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
