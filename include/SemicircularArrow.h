// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Claff for rendering an arrow that follows part of the arc of a circle.

#ifndef SEMI_CIRCULAR_ARROW_H
#define SEMI_CIRCULAR_ARROW_H

#include "PathTube.h"
#include "VisualObject.h"
#include "CircleIterator.h"

// =======================================================================================
// Class variable initialization

class SemicircularArrow: public PathTube, public VisualObject
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  SemicircularArrow(vec3 pos, vec3 axis, float arcD, float arcR, float tubeR, int s);
  ~SemicircularArrow(void);
  void generatePath(void);
  const char* objectName(void);
  inline void triangleBufferSizes(unsigned& vCount, unsigned& iCount)
    {PathTube::triangleBufferSizes(vCount, iCount);}
  inline bool bufferGeometry(TriangleBuffer* T)
    {return PathTube::bufferGeometry(T, zeroVec);}
  inline bool bufferGeometry(TriangleBuffer* T, vec3 offset)
    {return PathTube::bufferGeometry(T, offset);}
  inline bool matchRay(vec3& position, vec3& direction, float& lambda)
    {return PathTube::matchRay(position, direction, lambda, zeroVec);}
  inline bool matchRay(vec3& position, vec3& direction, float& lambda, vec3 offset)
    {return PathTube::matchRay(position, direction, lambda, offset);}
#ifdef LOG_TREE_VALIDATION
  void selfValidate(unsigned l);
#endif
  
private:
  
  // Instance variables - private
  CircleIterator circIter;
  float arcDegrees;
  float tubeRadius; 

  // Member functions - private
  SemicircularArrow(const SemicircularArrow&);                 // Prevent copy-construction
  SemicircularArrow& operator=(const SemicircularArrow&);      // Prevent assignment

};

#endif




