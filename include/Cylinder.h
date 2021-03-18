// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for rendering a cylinder

#ifndef CYLINDER_H
#define CYLINDER_H

#include "VisualElement.h"
#include "TruncatedCone.h"

// =======================================================================================
// Class variable initialization

class Cylinder: public VisualElement
{
 public:
  
  // Instance variables - public
  vec3      location;
  vec3      axisDirection;
  float     radius;
  unsigned  sides;
  unsigned  color;
  vec3      f1;    // f1 and f2 are cross-vectors to axisDirection
  vec3      f2;
#ifdef LOG_TREE_MATCH_RAY
  float     lastRayMatch;
  vec3      joinLine;
  vec3      originDiff;
#endif

  // Member functions - public
  Cylinder(vec3 root, vec3 dir, float R, unsigned sides);
  ~Cylinder(void);
  bool          updateBoundingBox(BoundingBox* box, vec3 offset);
  bool          bufferGeometry(TriangleBuffer* T, vec3 offset);
  bool          matchRay(vec3& position, vec3& direction, float& lambda);
  void          triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  void          lengthen(float increment);
  void          setLength(float length);
  inline float  getLength(void) {return glm_vec3_norm(axisDirection);}
  int           printOPSF(char*& buf, unsigned bufSize);
  const char*   objectName(void);
  bool          diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  Cylinder(const Cylinder&);                 // Prevent copy-construction
  Cylinder& operator=(const Cylinder&);      // Prevent assignment
};

#endif




