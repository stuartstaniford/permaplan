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
  vec3 controlPoints[4][4];         // i and j increase to the north and to the east
  vec3 copyOfControlPoints[4][4];   // used in fitting

  
  // Member functions - public
  BezierPatch(float x, float y, float width, float height,
                          float s, float t, float sWidth, float tHeight, unsigned gridPoints);
  BezierPatch(Quadtree* qtree, unsigned gridPoints);
  ~BezierPatch(void);
  void  triangleBufferSizes(unsigned& vertexCount, unsigned& indexCount);
  bool  bufferGeometry(TriangleBuffer* T);
  void  draw(void);
  void  surfacePoint(float x, float y, vec3 result);
  bool  matchRay(vec3& position, vec3& direction, float& lambda);
  void  updateBoundingBox(void);
  void  triangleBufferSize(unsigned& vCount, unsigned& iCount);
  void  randomFit(std::vector<float*>& locations);
  float estimateFit(std::vector<float*>& locations);
  bool  improveFit(std::vector<float*>& locations);
  void  setUpUVVals(std::vector<float*>& locations);
  void  copyFitPointUVVals(void);
  void  copyControlPoints(void);
  bool  diagnosticHTML(HttpDebug* serv);
  
private:
  
  // Instance variables - private
  unsigned              gridN;  // number of squares to divide u/v space into when tesselating.
  std::vector<float*>   fitPointUVVals;
  std::vector<float*>   copyOfFitPointUVVals; // used for fitting

  // Member functions - private
};

#endif


