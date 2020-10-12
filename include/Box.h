// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// Class for storing and rendering rectangular boxes (eg straw bales, durisol blocks,
// timber beams, etc).

#ifndef BOX_H
#define BOX_H

#include "VisualObject.h"

// =======================================================================================
// Class variable initialization


class Box: public VisualObject
{
 public:
  
  // Instance variables - public
  vec3 location;

  // Member functions - public
  Box(float x, float y, float z);
  Box(vec3 loc);
  ~Box(void);
  bool bufferGeometry(TriangleBuffer* T);
  void draw(void);
  bool matchRay(vec3& position, vec3& direction, float& lambda);
  void updateBoundingBox(void);
  void triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  const char* objectName(void);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
};

#endif




