// Copyright Staniford Systems.  All Rights Reserved.  July 2020 -
// Class for storing and rendering a section of the landsurface mesh for some
// particular region in the quadtree (generally a leaf).  This class defines
// the interface that any implementation providing this service needs to conform to.

#include "LandSurfaceRegion.h"
#include <err.h>


// =======================================================================================
/// @brief Constructor
/// @param

LandSurfaceRegion::LandSurfaceRegion(float x, float y, float width, float height,
                                     float s, float t, float sWidth, float tHeight):
                                                        VisualObject(true)
{
  xyPos[0]    = x;
  xyPos[1]    = y;
  extent[0]   = width;
  extent[1]   = height;
  stPos[0]    = s;
  stPos[1]    = t;
  stExtent[0] = sWidth;
  stExtent[1] = tHeight;
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);

  //updateBoundingBox();  // implementing subclass should do this once it's known.
}


// =======================================================================================
/// @brief Destructor

LandSurfaceRegion::~LandSurfaceRegion(void)
{
}


// =======================================================================================
// Obtains the height at a particular location.  This version uses matchRay and should
// work for any subclass that has implemented a proper matchRay.  Subclasses may wish
// to override for efficiency reasons.

#define HIGH_UP 20000.0f
#define ALT_STEP 1000.0f

float LandSurfaceRegion::getAltitude(float x, float y)
{
  vec3 position =  {x, y, HIGH_UP};
  vec3 direction = {0.0f, 0.0f, -ALT_STEP};
  float lambda;
  
  matchRayToObject(position, direction, lambda);
  
  return HIGH_UP - lambda*ALT_STEP;
}


// =======================================================================================
// Stub not done.

void  LandSurfaceRegion::fit(std::vector<float*>& locations)
{
  
}


// =======================================================================================
// Tell callers our name at runtime.

const char* LandSurfaceRegion::objectName(void)
{
  static char* name = (char*)"LandSurfaceRegion";
  return name;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool LandSurfaceRegion::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("<tr><td>LandSurfaceRegion</td><td>");
  httPrintf("<b>Why no subclass?\?\?</b><br>");
  httPrintf("<b>XY Position:</b> (%.1f, %.1f)<br>", xyPos[0], xyPos[1]);
  httPrintf("<b>Extent:</b> (%.1f, %.1f)<br>", extent[0], extent[1]);
  httPrintf("</td></tr>\n");
  
  return true;
}


// =======================================================================================
