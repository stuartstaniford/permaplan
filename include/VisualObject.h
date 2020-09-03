// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree


#ifndef VISUAL_OBJECT_H
#define VISUAL_OBJECT_H

#include "BoundingBox.h"
#include "TriangleBuffer.h"

// =======================================================================================
// Class variable initialization


class VisualObject
{
 public:
  
  // Instance variables - public
  BoundingBox*   box;

  // Member functions - public
  VisualObject(BoundingBox* B = NULL);
  virtual ~VisualObject(void);
  virtual bool bufferGeometry(TriangleBuffer* T);
  void draw(void);
  virtual bool matchRay(vec3& position, vec3& direction, float& lambda);
  virtual void updateBoundingBox(void);
  virtual void triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  virtual bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
};

#endif




