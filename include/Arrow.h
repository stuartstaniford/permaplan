// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Class for rendering a simple straight arrow.

#ifndef ARROW_H
#define ARROW_H

#include "VisualObject.h"

// =======================================================================================
// Class variable initialization


class Arrow: public VisualObject
{
 public:
  
  // Instance variables - public
  vec3 location;
  vec3 direction;
  
  // Member functions - public
  Arrow(vec3 root, vec3 dir);
  ~Arrow(void);
  bool bufferGeometry(TriangleBuffer* T);
  void draw(void);
  bool matchRay(vec3& position, vec3& direction, float& lambda);
  void updateBoundingBox(void);
  void triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
};

#endif




