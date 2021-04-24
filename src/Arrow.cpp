
// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Class for rendering a simple straight arrow.

#include <cstdio>
#include <err.h>
#include "Cylinder.h"
#include "Arrow.h"

vec2 Arrow::arrowPoints[3] = {  {0.5f, 0.0f},
                                    {0.5f, 0.75f},
                                    {1.0f, 0.75f}};

// =======================================================================================
// Constructors.

Arrow::Arrow(vec3 root, vec3 dir, float radius, int sides):
                      AxialElement(root, dir, radius, sides),
                      VisualObject(true)
{
  NVecs       = 3u;
  vectorPath  = arrowPoints;
  incrementArrowMemory(sizeof(Arrow)); //+sizeof(BoundingBox));
  closedBase = true;
  closedTop = true;
  box = new BoundingBox();
  AxialElement::updateBoundingBox(box, zeroVec); //XX can this zerovec here be right?
}


// =======================================================================================
// Destructor

Arrow::~Arrow(void)
{
  incrementArrowMemory(-sizeof(Arrow));
}


// =======================================================================================
// Tell callers our name at runtime.

const char* Arrow::objectName(void)
{
  static char* name = (char*)"Arrow";
  return name;
}


// =======================================================================================
// Function to validate the quadtree and all the objects.

#ifdef LOG_TREE_VALIDATION

void Arrow::selfValidate(unsigned l)
{
   box->selfValidate(true);
}

#endif


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.  This is the version called from Quadtree pages

bool Arrow::diagnosticHTMLSummary(HttpDebug* serv)
{
  httPrintf("<tr><td><a href=\"/object/%d\">Arrow</a></td>", objIndex);
  httPrintf("<td>Location: [%.1f, %.1f, %.1f]<br>Direction: [%.1f, %.1f, %.1f]</td>",
                      location[0], location[1], location[2], 
                      axisDirection[0], axisDirection[1], axisDirection[2]);  
  httPrintf("</tr>\n");

  return true;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular HeightMarker.
// XX needs to be fixed to be a proper page.

bool Arrow::diagnosticHTML(HttpDebug* serv)
{
  unless(serv->startResponsePage("Arrow"))
    return false;
  
  httPrintf("<b>Object Id:</b> %u<br>", objIndex);
  httPrintf("<b>Location:</b> (%.1f, %.1f, %.1f)<br>" ,
                                  location[0], location[1], location[2]);
  httPrintf("Direction:</b> (%.1f, %.1f, %.1f)</td>\n" ,
                           axisDirection[0], axisDirection[1], axisDirection[2]);
  
  unless(serv->newSection("Bounding Box"))
    return false;
  unless(box->diagnosticHTML(serv))
    return false;
  
  unless(serv->endResponsePage())
    return false;
  return true;
}


// =======================================================================================
