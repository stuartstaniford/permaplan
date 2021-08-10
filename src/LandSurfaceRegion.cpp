// Copyright Staniford Systems.  All Rights Reserved.  July 2020 -
// Class for storing and rendering a section of the landsurface mesh for some
// particular region in the quadtree (generally a leaf).  This class defines
// the interface that any implementation providing this service needs to conform to.

#include "LandSurfaceRegion.h"
#include <err.h>


// =======================================================================================
/// @brief Constructor.
///
/// @param x x-coordinate of the south-west corner of the covered region
/// @param y y-coordinate of the south-west corner of the covered region
/// @param width width of the covered region
/// @param height height of the covered region
/// @param s s texture-coordinate of the south-west corner of the covered region
/// @param t t texture-coordinate of the south-west corner of the covered region
/// @param sWidth width in texture space of the covered region
/// @param tHeight height in texture space of the covered region

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
/// @brief Implement the VisualElement interface.
/// This should be overwritten by implementing subclasses, but is required to be here so
/// that virtual inheritance works correctly for LandSurfaceRegion* pointers.

bool LandSurfaceRegion::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
/// @brief Implement the VisualElement interface.
/// This should be overwritten by implementing subclasses, but is required to be here so
/// that virtual inheritance works correctly for LandSurfaceRegion* pointers.

bool LandSurfaceRegion::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
/// @brief Implement the VisualElement interface.
/// This should be overwritten by implementing subclasses, but is required to be here so
/// that virtual inheritance works correctly for LandSurfaceRegion* pointers.

int LandSurfaceRegion::getNextIndex(bool resetToFirst)
{
  return -1;
}


// =======================================================================================
/// @brief Implement the VisualElement interface.
/// This should be overwritten by implementing subclasses, but is required to be here so
/// that virtual inheritance works correctly for LandSurfaceRegion* pointers.

bool LandSurfaceRegion::bufferGeometryOfObject(TriangleBuffer* T)
{
  err(-1, "Called unimplemented LandSurfaceRegion::bufferGeometry");
  return false;
}


// =======================================================================================
/// @brief Implement the VisualElement interface.
/// This should be overwritten by implementing subclasses, but is required to be here so
/// that virtual inheritance works correctly for LandSurfaceRegion* pointers.

void LandSurfaceRegion::triangleBufferSize(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
  err(-1, "Called unimplemented LandSurfaceRegion::triangleBufferSize");
}


// =======================================================================================
/// @brief Implement the VisualElement interface.
/// This should be overwritten by implementing subclasses, but is required to be here so
/// that virtual inheritance works correctly for LandSurfaceRegion* pointers.

bool LandSurfaceRegion::matchRayToObject(vec3& position, vec3& direction, float& lambda)
{
  if(!box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
/// @brief Obtain the height at a particular location.  
/// 
/// This version uses matchRay and should work for any subclass that has implemented a 
/// proper matchRay.  Subclasses may wish to override for efficiency reasons.
/// @param x x-coordinate of the location where we need the altitude.
/// @param y y-coordinate of the location where we need the altitude.

float LandSurfaceRegion::getAltitude(float x, float y)
{
  vec3 position =  {x, y, HIGH_UP};
  vec3 direction = {0.0f, 0.0f, -ALT_STEP};
  float lambda;
  
  matchRayToObject(position, direction, lambda);
  
  return HIGH_UP - lambda*ALT_STEP;
}


// =======================================================================================
/// @brief Implement the VisualElement interface.
/// This should be overwritten by implementing subclasses, but is required to be here so
/// that virtual inheritance works correctly for LandSurfaceRegion* pointers.
                                                                
void LandSurfaceRegion::updateBoundingBox(void)
{
  if(!box)
    box = new BoundingBox(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  return;
}


// =======================================================================================
/// @brief Stub not done.

void  LandSurfaceRegion::fit(std::vector<float*>& locations)
{
  
}


// =======================================================================================
/// @brief Tell callers our name at runtime.

const char* LandSurfaceRegion::objectName(void)
{
  static char* name = (char*)"LandSurfaceRegion";
  return name;
}


// =======================================================================================
/// @brief Stub definition this should be overwritten by implementing subclasses

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
