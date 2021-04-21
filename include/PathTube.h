// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Class for rendering an element that consists of a tube (of varying width) that
// follows a specified path (set of points).  Provides support to things like 
// semicircular arrows.

#ifndef PATH_TUBE_H
#define PATH_TUBE_H

#include "VisualElement.h"

// =======================================================================================
// Class variable initialization

class PathTube: public VisualElement
{
public:
  
  // Instance variables - public
  unsigned        color;
  bool            closedBase;
  bool            closedTop;
  
  // Member functions - public
  PathTube(void);
  ~PathTube(void);
  virtual void triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  virtual bool bufferGeometry(TriangleBuffer* T, vec3 offset);
  virtual bool matchRay(vec3& position, vec3& direction, float& lambda, vec3 offset);
  virtual bool updateBoundingBox(BoundingBox* box, vec3 offset);

private:
  
  // Instance variables - private
  vec4*     path;
  unsigned  NPath;
  unsigned  sides;
  
  // Member functions - private
  PathTube(const PathTube&);                 // Prevent copy-construction
  PathTube& operator=(const PathTube&);      // Prevent assignment

};

#endif




