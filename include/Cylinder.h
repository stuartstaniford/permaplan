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
  vec3      direction;
  float     radius;
  unsigned  sides;
  
  // Member functions - public
  Cylinder(vec3 root, vec3 dir, float R, unsigned sides);
  ~Cylinder(void);
  bool          getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  bool          getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  int           getNextIndex(bool resetToFirst);
  bool          bufferGeometry(TriangleBuffer* T, float altitude);
  bool          matchRay(vec3& position, vec3& direction, float& lambda);
  void          triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  void          lengthen(float increment);
  void          setLength(float length);
  inline float  getLength(void) {return glm_vec3_norm(direction);}
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

void getCrossVectors(vec3 dir, vec3 f1, vec3 f2);


#endif




