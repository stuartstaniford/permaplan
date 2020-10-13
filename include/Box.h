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
  mat4 trans;

  // Member functions - public
  Box(mat4 transform);   // We take the unit box on [0,0,0] to [1,1,1] and apply transform to it.
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




