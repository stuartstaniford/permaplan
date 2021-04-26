// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class allows us to create a series of points (and normals to those points)
// around a circle or arc of a circle.  Was developed because the same kind of code
// was showing up again and again, and this was a place to consolidate it.  We try
// to minimize the copying of state.

#ifndef CIRCLE_ITERATOR_H
#define CIRCLE_ITERATOR_H

#include <cglm/cglm.h>

// =======================================================================================
// Class variable initialization

class CircleIterator
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  CircleIterator(float* centroid, float* dir, float R, float* offset = NULL);
  CircleIterator(void);
  ~CircleIterator(void);
  void update(float* centroid, float* dir, float R, float* offset = NULL);
  void getPoint(float degrees, float* pos, float* norm);
  
private:
  
  // Instance variables - private
  vec3  f1;
  vec3  f2;
  float cosAng;
  float sinAng;
  float arcRadius;
  float* center;
  float* direction;
  
  
  // Member functions - private
  CircleIterator(const CircleIterator&);                 // Prevent copy-construction
  CircleIterator& operator=(const CircleIterator&);      // Prevent assignment
  
};

#endif


// =======================================================================================
// Utility function declarations
// XX ultimately should be able to hide these and only have them called locally.

void  getCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius);
void  updateCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius);

