// Copyright Staniford Systems.  All Rights Reserved.  Aug 2020 -
// A set of control points to manage a cubic bezier patch on some particular
// node in the Quadtree


#ifndef BEZIER_PATCH_H
#define BEZIER_PATCH_H

#include "LandSurfaceRegion.h"
#include "Quadtree.h"

// =======================================================================================
// Class variable initialization


class BezierPatch: public LandSurfaceRegion
{
public:
  
  // Instance variables - public
  vec3 controlPoints[4][4];     // i and j increase to the north and to the east
  
  
  // Member functions - public
  BezierPatch(float x, float y, float width, float height,
                          float s, float t, float sWidth, float tHeight);
  BezierPatch(Quadtree* qtree);
  ~BezierPatch(void);
  bool  bufferGeometry(TriangleBuffer* T);
  void  draw(void);
  void surfacePoint(vec3 result, float x, float y);
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


