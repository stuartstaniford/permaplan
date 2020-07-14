// Copyright Staniford Systems.  All Rights Reserved.  July 2020 -
// Class for storing and rendering a section of the landsurface mesh for some
// particular region in the quadtree (generally a leaf).  This class defines
// a simple planar version of the interface, with the plane defined by the
// constructor variables pos and norm - plane passes through pos and is perpendicular
// to norm.

#include <err.h>
#include "LandSurfaceRegionPlanar.h"

// =======================================================================================
// Constructors.

LandSurfaceRegionPlanar::LandSurfaceRegionPlanar(float x, float y, float width, float height,
                           vec3 plane):LandSurfaceRegion(x,y,width,height)
{
}


// =======================================================================================
// Destructor

LandSurfaceRegionPlanar::~LandSurfaceRegionPlanar(void)
{
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool LandSurfaceRegionPlanar::bufferGeometry(TriangleBuffer* T)
{


  return false;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void LandSurfaceRegionPlanar::triangleBufferSize(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void LandSurfaceRegionPlanar::draw(void)
{
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool LandSurfaceRegionPlanar::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(!box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
// Stub not done.
                                                                
void LandSurfaceRegionPlanar::updateBoundingBox(void)
{
  if(!box)
    box = new BoundingBox(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  return;
}
                                                                

// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool LandSurfaceRegionPlanar::diagnosticHTML(HttpDebug* serv)
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
