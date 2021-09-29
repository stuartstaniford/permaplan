// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -

#ifndef SEMI_CIRCULAR_ARROW_H
#define SEMI_CIRCULAR_ARROW_H

#include "PathTube.h"
#include "VisualObject.h"
#include "CircleIterator.h"


// =======================================================================================
/// @brief Render an arrow that follows part of the arc of a circle.
///
/// Gets most of the actual implementation functionality from PathTube, and also 
/// implements the VisualObject interface, as this will show up in ControlGroups in
/// the Quadtree.

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
  inline bool bufferGeometryOfObject(TriangleBuffer* T)
    {return PathTube::bufferGeometryOfElement(T, zeroVec);}
  inline bool matchRayToObject(vec3& position, vec3& direction, float& lambda)
    {return PathTube::matchRayToElement(position, direction, lambda, zeroVec);}
  inline virtual unsigned getObjectIndex(void) {return objIndex;}
  void selfValidate(unsigned l);
  
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




