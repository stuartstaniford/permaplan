// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is a subclass of ObjectGroup specifically to handle the visual controls
// that appear around an object after it has been double-clicked.

#include "ControlGroup.h"
#include "Arrow.h"
#include "PmodDesign.h"


// =======================================================================================
// Constructor

ControlGroup::ControlGroup(VisualObject* firstObject):
                                  ObjectGroup(firstObject)
{
  masterObject = firstObject; // the main object - everything else is ephemeral controls
  LogControlGroupInit("Started new control group around object of type %s.\n",
                        firstObject->objectName());
  
  createTranslationArrows();
}


// =======================================================================================
// Destructor

ControlGroup::~ControlGroup(void)
{
}


// =======================================================================================
// Create the horizonal movement arrows

void ControlGroup::createTranslationArrows(void)
{
  // Initial setup for all four arrows
  PmodDesign& design = PmodDesign::getDesign();
  float arrowLength = masterObject->box->height()*0.5f;
  float arrowSpacing = arrowLength*0.4f;
  vec3 pos, dir;
  vec3 centroid;
  masterObject->box->getCentroid(centroid);
  pos[2] = centroid[2];
  dir[2] = 0.0f;
  Arrow* arrow;
  
  // North arrow
  pos[0] = centroid[0];
  dir[0] = 0.0f;
  pos[1] = masterObject->box->upper[1] + arrowSpacing;
  dir[1] = arrowLength;
  arrow = new Arrow(pos, dir, design.spaceUnitsFromFeet(1.0f), 10);
  insert(arrow);
  *box += *(arrow->box);
  
  // South arrow
  pos[1] = masterObject->box->lower[1] - arrowSpacing;
  dir[1] = -arrowLength;
  arrow = new Arrow(pos, dir, design.spaceUnitsFromFeet(1.0f), 10);
  insert(arrow);
  *box += *(arrow->box);

  // East arrow
  pos[0] = masterObject->box->upper[1] + arrowSpacing;
  dir[0] = arrowLength;
  pos[1] = centroid[1];
  dir[1] = 0.0f;
  arrow = new Arrow(pos, dir, design.spaceUnitsFromFeet(1.0f), 10);
  insert(arrow);
  *box += *(arrow->box);

  // West arrow
  pos[0] = masterObject->box->lower[1] - arrowSpacing;
  dir[0] = -arrowLength;
  arrow = new Arrow(pos, dir, design.spaceUnitsFromFeet(1.0f), 10);
  insert(arrow);
  *box += *(arrow->box);
}


// =======================================================================================
// Tell callers our name at runtime.

const char* ControlGroup::objectName(void)
{
  static char* name = (char*)"Control Group";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.  This is the version called from Quadtree pages

bool ControlGroup::diagnosticHTMLSummary(HttpDebug* serv)
{
  httPrintf("<tr><td>Control Group</a></td>");
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
