// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for rendering a cylinder

#include "Cylinder.h"
#include <cstdio>
#include <err.h>


// =======================================================================================
/// @brief Constructor.
/// @param root A vector to the base of the axis of the element.
/// @param dir A vector of the length and direction of the axis of the element.
/// @param R The (float) radius of the cylinder.
/// @param S An unsigned for the number of sides to use in approximating the cylinder.

Cylinder::Cylinder(vec3 root, vec3 dir, float R, unsigned S):
                                              AxialElement(root, dir, R, S),
                                              cylinderPoints{{1.0f, 0.0f}, {1.0f, 1.0f}}
{
  NVecs       = 2u;
  vectorPath  = cylinderPoints;
}


// =======================================================================================
/// @brief Destructor

Cylinder::~Cylinder(void)
{
}


// =======================================================================================
/// @brief Function to print out in OPSF/JSON format.
/// @param buf A reference to a char* pointer for the buffer to write to
/// @param bufSize The remaining size of the buffer.
/// @todo Function is currently a stub.

int Cylinder::printOPSF(char*& buf, unsigned bufSize)
{
  char* end = buf + bufSize;

  return (bufSize - (int)(end-buf));
}


// =======================================================================================
/// @brief Tell callers our name at runtime.

const char* Cylinder::objectName(void)
{
  static char* name = (char*)"Cylinder";
  return name;
}


// =======================================================================================
/// @brief Report on our object to an HTTP page.
/// 
/// We assume we are part of a table of visual objects and we just contribute 
/// one row about this particular object.  This is the version called from Quadtree pages
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

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
