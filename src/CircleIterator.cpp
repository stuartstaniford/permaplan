// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class allows us to create a series of points (and normals to those points)
// around a circle or arc of a circle.  Was developed because the same kind of code
// was showing up again and again, and this was a place to consolidate it.

#include "CircleIterator.h"
#include "Global.h"

// =======================================================================================
// Constructor

CircleIterator::CircleIterator(void)
{
}


// =======================================================================================
// Destructor

CircleIterator::~CircleIterator(void)
{
}


// =======================================================================================
// Utility function to get two vectors perpendicular to a specified one.
// Eg if sides == 8 the cross section of the TruncatedCone which points in the dir
// direction looks like this (dir points into the screen):
/*
     ^
     | f1 direction
 
   -----
  /     \
 /       \
|         |  ---->
|         |  f2 direction
 \       /
  \     /
   -----
*/

void getCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius)
{
  vec3 f0; // starting place, will be z-axis unless dir is parallel when we use x-axis.

  if(fabs(dir[0]) < EPSILON && fabs(dir[1]) < EPSILON)
   {
    f0[0] = 1.0f;
    f0[1] = f0[2] = 0.0f;
   }
  else
   {
    f0[0] = f0[1] = 0.0f;
    f0[2] = 1.0f;
   }

  glm_vec3_cross(f0, dir, f1);
  glm_vec3_cross(dir, f1, f2);
  glm_vec3_scale_as(f1, radius, f1);
  glm_vec3_scale_as(f2, radius, f2);
}


// =======================================================================================
// This function is used te ensure the stability of f1, f2 as we go up the tube, we use the 
// last f1,f2 as the starting point for the next set. 


void updateCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius)
{
  glm_vec3_cross(dir, f1, f2);
  glm_vec3_cross(f2, dir, f1);
  glm_vec3_scale_as(f1, radius, f1);
  glm_vec3_scale_as(f2, radius, f2);
}


// =======================================================================================
