// Copyright Staniford Systems.  All Rights Reserved.  July 2020 -
// Class for storing and rendering a section of the landsurface mesh for some
// particular region in the quadtree (generally a leaf).  This class defines
// a simple planar version of the interface, but it can be overriden by
// more advanced subclasses that implement a more complex surface model.

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
  
  updateBoundingBox();
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
/*
  serv->addResponseData("<tr><td>HeightMarker</td>");
  serv->respPtr += sprintf(serv->respPtr,
                           "<td><b>Location:</b> (%.1f, %.1f, %.1f)</td></tr>\n",
                           location[0], location[1], location[2]);
*/
 return true;
}


// =======================================================================================
