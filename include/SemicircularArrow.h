// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Claff for rendering an arrow that follows part of the arc of a circle.

#ifndef SEMI_CIRCULAR_ARROW_H
#define SEMI_CIRCULAR_ARROW_H

#include "PathTube.h"
#include "VisualObject.h"

// =======================================================================================
// Class variable initialization

class SemicircularArrow: public PathTube, public VisualObject
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  SemicircularArrow(vec3 pos, vec3 axis, float arcD, float arcR, float tubeR, int s);
  ~SemicircularArrow(void);
  const char* objectName(void);
#ifdef LOG_TREE_VALIDATION
  void selfValidate(unsigned l);
#endif
  
private:
  
  // Instance variables - private
  vec3 centroid;
  vec3 axisDirection;
  float arcDegrees;
  float arcRadius; 
  float tubeRadius; 
  int sides;

  // Member functions - private
  SemicircularArrow(const SemicircularArrow&);                 // Prevent copy-construction
  SemicircularArrow& operator=(const SemicircularArrow&);      // Prevent assignment

};

#endif




