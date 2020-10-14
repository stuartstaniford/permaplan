// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// Class for storing and rendering the user supplied locations of height markers.
// Renders them as an octahedron balanced on the actual height location

#ifndef HEIGHT_MARKER_H
#define HEIGHT_MARKER_H

#include "VisualObject.h"

// =======================================================================================
// Class variable initialization


class HeightMarker: public VisualObject
{
 public:
  
  // Instance variables - public
  vec3 location;

  // Member functions - public
  HeightMarker(float x, float y, float z);
  HeightMarker(vec3 loc);
  ~HeightMarker(void);
  float*      getNextVertex(bool firstVertex);  // return is ptr to a vec3
  bool        bufferGeometry(TriangleBuffer* T);
  void        draw(void);
  bool        matchRay(vec3& position, vec3& direction, float& lambda);
  void        updateBoundingBox(void);
  void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  const char* objectName(void);
  bool        diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
};

#endif




