// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for rendering a cylinder

#ifndef CYLINDER_H
#define CYLINDER_H

#include "AxialElement.h"
#include "TruncatedCone.h"

// =======================================================================================
// Class variable initialization

class Cylinder: public AxialElement
{
 public:
  
  // Instance variables - public
  float     radius;

  // Member functions - public
  Cylinder(vec3 root, vec3 dir, float R, unsigned sides);
  ~Cylinder(void);
  bool          updateBoundingBox(BoundingBox* box, vec3 offset);
  bool          bufferGeometry(TriangleBuffer* T, vec3 offset);
  bool          matchRay(vec3& position, vec3& direction, float& lambda, vec3 offset);
  bool          matchRayBruteForce(vec3& position, vec3& direction,
                                                            float& lambda, vec3 offset);
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




