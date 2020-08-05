// Copyright Staniford Systems.  All Rights Reserved.  July 2020 -
// Class for storing and rendering a section of the landsurface mesh for some
// particular region in the quadtree (generally a leaf).  This class defines
// a simple planar version of the interface, with the plane defined by the
// constructor variables pos and norm - plane passes through pos and is perpendicular
// to norm.

#ifndef LAND_SURFACE_REGION_PLANAR_H
#define LAND_SURFACE_REGION_PLANAR_H

#include "LandSurfaceRegion.h"

// =======================================================================================
// Class variable initialization


class LandSurfaceRegionPlanar: public LandSurfaceRegion
{
 public:
  
  // Instance variables - public
  vec4 heights; 

  
  // Member functions - public
  LandSurfaceRegionPlanar(float x, float y, float width, float height,
                          float s, float t, float sWidth, float tHeight, vec3 plane);
  ~LandSurfaceRegionPlanar(void);
  void  resetPlane(vec3 plane);
  bool  bufferGeometry(TriangleBuffer* T);
  void  draw(void);
  bool  matchRay(vec3& position, vec3& direction, float& lambda);
  void  updateBoundingBox(void);
  void  triangleBufferSize(unsigned& vCount, unsigned& iCount);
  void  fit(std::vector<float*>& locations);
  bool  diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
};

#endif




