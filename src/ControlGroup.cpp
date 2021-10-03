// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is a subclass of ObjectGroup specifically to handle the visual controls
// that appear around an object after it has been double-clicked.

#include "ControlGroup.h"
#include "Arrow.h"
#include "PmodDesign.h"
#include "SemicircularArrow.h"
#include "HttpDebug.h"


// =======================================================================================
/// @brief Constructor
/// @param firstObject A pointer to a VisualObject which is the thing about which this
/// this control group will be centered.

ControlGroup::ControlGroup(VisualObject* firstObject):
                                  ObjectGroup(firstObject)
{
  masterObject = firstObject; // the main object - everything else is ephemeral controls
  LogControlGroupInit("Started new control group around object of type %s.\n",
                        firstObject->objectName());
  
  createTranslationArrows();
  createRotationArrows();
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
}


// =======================================================================================
/// @brief Destructor

ControlGroup::~ControlGroup(void)
{
}

// =======================================================================================

unsigned arrowRed = 0xff2200ff;


// =======================================================================================
/// @brief Add one arrow into ourself.
/// @param pos The vec3 position of the base of the arrow
/// @param dir The vec3 direction vector from base to head of arrow.

void ControlGroup::addOneArrow(vec3 pos, vec3 dir)
{
  PmodDesign& design = PmodDesign::getDesign();
  Arrow* arrow = new Arrow(pos, dir, design.spaceUnitsFromFeet(1.0f), 10);
  arrow->color = arrowRed;
  insert(arrow);
  *box += *(arrow->box);
}


// =======================================================================================
/// @brief Create the four horizonal movement arrows to translate the object.
///
/// One is created for each of the north, south, east, and west directions.

void ControlGroup::createTranslationArrows(void)
{
  // Initial setup for all four arrows
  float arrowLength = masterObject->box->height()*0.5f;
  float arrowSpacing = arrowLength*0.4f;
  vec3 pos, dir;
  vec3 centroid;
  masterObject->box->getCentroid(centroid);
  pos[2] = centroid[2];
  dir[2] = 0.0f;
  
  // North arrow
  pos[0] = centroid[0];
  dir[0] = 0.0f;
  pos[1] = masterObject->box->upper[1] + arrowSpacing;
  dir[1] = arrowLength;
  addOneArrow(pos, dir);
  
  // South arrow
  pos[1] = masterObject->box->lower[1] - arrowSpacing;
  dir[1] = -arrowLength;
  addOneArrow(pos, dir);

  // East arrow
  pos[0] = masterObject->box->upper[0] + arrowSpacing;
  dir[0] = arrowLength;
  pos[1] = centroid[1];
  dir[1] = 0.0f;
  addOneArrow(pos, dir);

  // West arrow
  pos[0] = masterObject->box->lower[0] - arrowSpacing;
  dir[0] = -arrowLength;
  addOneArrow(pos, dir);
}


// =======================================================================================
/// @brief Create the semi-circular arrows allowing us to rotate the object around 
/// a vertical axis

void ControlGroup::createRotationArrows(void)
{
  vec3 centroid;
  masterObject->box->getCentroid(centroid);
  vec3 dir = {0.0f, 0.0f, 1.0f};
  float arcR = 0.8f*masterObject->box->avgDiam();

  // Rotate clockwise arrow
  SemicircularArrow* arrow = new SemicircularArrow(centroid, dir, 60.0f, arcR, 1.0f, 10);
  arrow->color = arrowRed;
  insert(arrow);
  *box += *(arrow->box);
}


// =======================================================================================
/// @brief Tell callers our name at runtime.

const char* ControlGroup::objectName(void)
{
  static char* name = (char*)"Control Group";
  return name;
}


// =======================================================================================
/// @brief Report on our object to an HTTP page.
/// 
/// We assume we are part of a table of visual objects and we just contribute 
/// one row about this particular object.  This is the version called from Quadtree pages
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool ControlGroup::diagnosticHTMLSummary(HttpDebug* serv)
{
  httPrintf("<tr><td><a href=\"/object/%d\">Control Group</a></td>", objIndex);
  httPrintf("<td>");
  unless(serv->startTable())
    return false;
  httPrintf("<tr><th>Type</th><th>Details</th></tr>");
  
  for(VisualObject* v: *this)
    unless(v->diagnosticHTMLSummary(serv))
      return false;

  httPrintf("</table></td></tr>\n");

  return true;
}


// =======================================================================================
