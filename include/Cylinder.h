// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for rendering a simple straight arrow.

#ifndef CYLINDER_H
#define CYLINDER_H

#include "VisualObject.h"

// =======================================================================================
// Class variable initialization

class Cylinder
{
 public:
  
  // Instance variables - public
  vec3      location;
  vec3      axisDirection;
  float     radius;
  unsigned  sides;
  vec3      f1;    // f1 and f2 are cross-vectors to axisDirection
  vec3      f2;

  // Member functions - public
  Cylinder(vec3 root, vec3 dir, float R, unsigned sides);
  ~Cylinder(void);
  bool          getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  bool          getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  bool          updateBoundingBox(BoundingBox* box, float altitude);
  int           getNextIndex(bool resetToFirst);
  bool          bufferGeometry(TriangleBuffer* T, float altitude, unsigned color);
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


// =======================================================================================
// Utility function declarations

void  getCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius);


#endif




