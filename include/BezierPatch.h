// Copyright Staniford Systems.  All Rights Reserved.  Aug 2020 -
// A set of control points to manage a cubic bezier patch on some particular
// node in the Quadtree


#ifndef BEZIER_PATCH_H
#define BEZIER_PATCH_H

#include "LandSurfaceRegion.h"
#include "Quadtree.h"
#include "DisplayList.h"

//#define BEZIER_DUMP_DETAIL
#define VISUALIZE_FITTING 1

// =======================================================================================
// Class variable initialization

class PatchRayState; // forward declaration, see bottom of file

// =======================================================================================
// Main class for a BezierPatch

class BezierPatch: public LandSurfaceRegion
{
public:
  
  // Instance variables - public
  vec3 controlPoints[4][4];         // i and j increase to the north and to the east
  vec3 copyOfControlPoints[4][4];   // used in fitting
  vec3 gradientControlPoints[4][4]; // direction in which fit distance increases fastest

  
  // Member functions - public
  BezierPatch(float x, float y, float width, float height,
                          float s, float t, float sWidth, float tHeight, unsigned gridPoints);
  BezierPatch(Quadtree* qtree, unsigned gridPoints);
  ~BezierPatch(void);
  void          computeGradConstraints(void);
  void          calcPowers(float u, float v);
  void          triangleBufferSizes(unsigned& vertexCount, unsigned& indexCount);
  bool          bufferGeometry(TriangleBuffer* T);
  void          draw(void);
  void          surfacePoint(float x, float y, vec3 result);
  bool          matchRay(vec3& position, vec3& direction, float& lambda);
  void          updateBoundingBox(void);
  DisplayList*  newUVLocationList(void);
  void          addControlPointsToDisplayList(DisplayList* D);
  void          addControlGradientsToDisplayList(DisplayList* D);
  void          triangleBufferSize(unsigned& vCount, unsigned& iCount);
  void          randomFit(std::vector<float*>& locations);
  void          levelFit(std::vector<float*>& locations);
  void          readControlPointsFromFile(FILE* file);
  bool          improveFit(std::vector<float*>& locations);
  const char*   objectName(void);
  void          printControlPoints(FILE* file);
  void          assertCopyVer(void);
  bool          diagnosticHTML(HttpDebug* serv);


private:
  
  // Instance variables - private
  unsigned              gridN;  // number of squares to divide u/v space into when tesselating.
  std::vector<float*>   fitPointUVVals;
  std::vector<float*>   copyOfFitPointUVVals; // used for fitting
  std::vector<float*>   gradientFitPointUVVals; // used for fitting

  // Used repeatedly for calculating Bernstein polynomial expressions
  float upow[4], vpow[4], u1minpow[4], v1minpow[4];
  double currentDelta;
  PatchRayState*        lastRayMatch;
  
#ifdef BEZIER_DUMP_DETAIL
  int   fitIterationCount;
#endif
  
  // Member functions - private
  float estimateFit(std::vector<float*>& locations);
  void  setUpUVVals(std::vector<float*>& locations);
  void  copyFitPointUVVals(void);
  void  copyControlPoints(void);
  void  computeGradientVector(std::vector<float*>& locations);
  void  applyGradientVector(void);
  void  revertGradientVector(void);
  void  dumpDetailState(char* fileName);
  bool  matchRayAll(vec3& position, vec3& direction, float& lambda);

};


// =======================================================================================
// Utility functions not part of class

void printControlPointArray(FILE* file, char* title, vec3 myArray[4][4]);

// =======================================================================================
// Helper state for tracking a ray traversing the patch (typically the mouse).
// The basic idea here is that rather than do a ton of complicated algebraic geometry,
// we rely on the fact that most of the time the ray points to the same triangle as
// in the last frame.  If it doensn't, it very likely points to a neighboring triangle.
// So almost all the time we can get by with a few quick triangle tests.  Only
// very occasionally do we pay the high cost of doing a brute force search of all the
// triangles (which will be amortized away).  Even in the scenario where the mouse
// is within the Bezier patch AABB, but not intersecting the patch, we can track the
// triangle which *would have been closest*, and if the distance to that has barely
// changed, we can figure there's no need to do the work of searching all the triangles
// again.  Only when the mouse is on the move in this zone do we need to do more work.

class PatchRayState
{
  friend class BezierPatch;
  
  vec3        triangle[3];
  vec2        uv;
  float       spacing;
  BezierPatch* parent;
  bool        lowerLeft;
  bool        validMatch;
  
  inline bool matchRay(vec3 rayPos, vec3 rayDir, float& outT)
   {
    return mollerTrumbore(triangle, rayPos, rayDir, outT);
   }
  
  inline void getUpperRight(vec3 outTriangle[3])
   {
    glm_vec3_copy(triangle[1], outTriangle[0]);
    parent->surfacePoint(uv[0]+spacing, uv[1]+spacing, outTriangle[1]);
    glm_vec3_copy(triangle[2], outTriangle[2]);
   }
  
  inline void getLowerLeft(vec3 outTriangle[3])
   {
    parent->surfacePoint(uv[0], uv[1], outTriangle[0]);
    glm_vec3_copy(triangle[0], outTriangle[1]);
    glm_vec3_copy(triangle[2], outTriangle[2]);
   }

  inline void getLowerLeft(vec3 outTriangle[3], float u, float v)
   {
    parent->surfacePoint(uv[0],           uv[1],            outTriangle[0]);
    parent->surfacePoint(uv[0] + spacing, uv[1],            outTriangle[1]);
    parent->surfacePoint(uv[0],           uv[1] + spacing,  outTriangle[2]);
   }

  inline void getUpperRight(vec3 outTriangle[3], float u, float v)
   {
    parent->surfacePoint(uv[0] + spacing, uv[1],            outTriangle[0]);
    parent->surfacePoint(uv[0] + spacing, uv[1] + spacing,  outTriangle[1]);
    parent->surfacePoint(uv[0],           uv[1] + spacing,  outTriangle[2]);
   }

  bool matchNeighbor(vec3 rayPos, vec3 rayDir, float& outT);
  
};
#endif


