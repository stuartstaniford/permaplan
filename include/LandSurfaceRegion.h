// Copyright Staniford Systems.  All Rights Reserved.  July 2020 -
// Class for storing and rendering a section of the landsurface mesh for some
// particular region in the quadtree (generally a leaf).  This class defines
// a simple planar version of the interface, but it can be overriden by
// more advanced subclasses that implement a more complex surface model.

#ifndef LAND_SURFACE_REGION_H
#define LAND_SURFACE_REGION_H

#include "VisualObject.h"

// =======================================================================================
// Class variable initialization


class LandSurfaceRegion: public VisualObject
{
 public:
  
  // Instance variables - public
  vec2 xyPos;
  vec2 extent;

  // Member functions - public
  LandSurfaceRegion(float x, float y, float width, float height);
  ~LandSurfaceRegion(void);
  bool bufferGeometry(TriangleBuffer* T);
  void draw(void);
  bool matchRay(vec3& position, vec3& direction, float& lambda);
  void updateBoundingBox(void);
  void triangleBufferSize(unsigned& vCount, unsigned& iCount);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
};

#endif




