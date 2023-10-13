
// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Class for rendering a simple straight arrow.

#include "Arrow.h"
#include "Cylinder.h"
#include "HttpDebug.h"
#include <cstdio>
#include <err.h>
#include <assert.h>

vec2 Arrow::arrowPoints[3] = {  {0.5f, 0.0f},
                                    {0.5f, 0.75f},
                                    {1.0f, 0.75f}};

// =======================================================================================
/// @brief Constructor for an Arrow
///
/// Most of the actual functionality for Arrow is inherited from AxialElement, and the
/// constructor here is mainly concerned with setting up the AxialElement::vectorPath and
/// other parameters.
///
/// @param root A vector to the base of the axis of the arrow.
/// @param dir A vector of the length and direction of the axis of the arrow.
/// @param radius The (float) radius of the widest point of the arrow (the base of the 
/// head. 
/// @param sides An int for the number of sides to use in approximating the sweep of the
/// arrow around the axis.  Making this bigger will make the arrow look smoother,
/// especially at close range, but will have a performance impact.

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
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
}


// =======================================================================================
/// @brief Destructor.  Currently just decrements ArrowMemory.

Arrow::~Arrow(void)
{
  incrementArrowMemory(-sizeof(Arrow));
}


// =======================================================================================
/// @brief Tell callers our name at runtime.

const char* Arrow::objectName(void)
{
  static char* name = (char*)"Arrow";
  return name;
}


// =======================================================================================
/// @brief Function to validate the Arrow (called from Quadtree::selfValidate).
///
/// Currently validates the bounding box, and tests that the length of the arrow is greater
/// than zero.
/// @param l Required by the VisualElement interface but unused here.


void Arrow::selfValidate(unsigned l)
{
#ifdef LOG_TREE_VALIDATION
   box->selfValidate(true);
   assert(getLength() > 0.0f);
#endif
}



// =======================================================================================
/// @brief Provide single row summary of this particular Arrow.
/// 
/// We assume we are part of a table of visual objects and we just contribute one row
/// about this particular object.  This is the version called from Quadtree pages
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.

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
/// @brief Provide an HTML page to the HTTP diagnostic server for our objIndex
///
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.

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
