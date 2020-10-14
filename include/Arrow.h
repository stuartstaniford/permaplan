// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Class for rendering a simple straight arrow.

#ifndef ARROW_H
#define ARROW_H

#include "VisualObject.h"

// =======================================================================================
// Class variable initialization

//XX this should perhaps be a template parameter to the class
#define ARROW_SIDES 6
#define ARROW_RADIUS 0.07        // as a multiple of length (which is norm(direction)).
#define ARROW_HEAD 0.25          // fraction of the arrow in the head.
#define ARROW_HEAD_WIDTH 1.0f    // multiple of the radius for the overhang over the shaft.

class Arrow: public VisualObject
{
 public:
  
  // Instance variables - public
  vec3 location;
  vec3 direction;
  
  // Member functions - public
  Arrow(vec3 root, vec3 dir);
  ~Arrow(void);
  float* getNextVertex(bool firstVertex);  // return is ptr to a vec3
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




