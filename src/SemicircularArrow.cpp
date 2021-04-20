// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class ..... is a template for real classes

#include "SemicircularArrow.h"

// =======================================================================================
// Constructor

SemicircularArrow::SemicircularArrow(vec3 pos, vec3 axis, 
                                     float arcD, float arcR, float tubeR, int s):
                                      arcDegrees(arcD),
                                      arcRadius(arcR), 
                                      tubeRadius(tubeR), 
                                      sides(s)
{
  glm_vec3_copy(pos, centroid);
  glm_vec3_copy(axis, axisDirection);

}


// =======================================================================================
// Destructor

SemicircularArrow::~SemicircularArrow(void)
{
}


// =======================================================================================
