// Copyright Staniford Systems.  All Rights Reserved.  July 2020 -
// Class for storing and rendering a section of the landsurface mesh for some
// particular region in the quadtree (generally a leaf).  This class defines
// the interface that any implementation providing this service needs to conform to.

#include <err.h>
#include "LandSurfaceRegion.h"

// =======================================================================================
// Constructors.

LandSurfaceRegion::LandSurfaceRegion(float x, float y, float width, float height)
{
  xyPos[0]  = x;
  xyPos[1]  = y;
  extent[0] = width;
  extent[1] = height;
  
  //updateBoundingBox();  // implementing subclass should do this once it's known.
}


// =======================================================================================
// Destructor

LandSurfaceRegion::~LandSurfaceRegion(void)
{
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool LandSurfaceRegion::bufferGeometry(TriangleBuffer* T)
{


  return false;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void LandSurfaceRegion::triangleBufferSize(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void LandSurfaceRegion::draw(void)
{
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool LandSurfaceRegion::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(!box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
// Stub not done.
                                                                
void LandSurfaceRegion::updateBoundingBox(void)
{
  if(!box)
    box = new BoundingBox(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  return;
}
                                                                

// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool LandSurfaceRegion::diagnosticHTML(HttpDebug* serv)
{
  serv->addResponseData("<tr><td>LandSurfaceRegion</td><td>");
  serv->addResponseData("<b>Why no subclass???</b><br>");
  serv->respPtr += sprintf(serv->respPtr, "<b>XY Position:</b> (%.1f, %.1f)<br>",
                           xyPos[0], xyPos[1]);
  serv->respPtr += sprintf(serv->respPtr, "<b>Extent:</b> (%.1f, %.1f)<br>",
                           extent[0], extent[1]);
  serv->addResponseData("</td></tr>\n");
  
  return true;
}


// =======================================================================================
