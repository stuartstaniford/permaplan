// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -

#ifndef CIRCLE_ITERATOR_H
#define CIRCLE_ITERATOR_H

#include <cglm/cglm.h>


// =======================================================================================
/// @brief This class allows us to create a series of points (and normals to those 
/// points) around a circle or arc of a circle.  
///
/// This was developed because the same kind of code was showing up again and again, and 
/// this was a place to consolidate it.  We try to minimize the copying of state.

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

// =======================================================================================

#endif
