// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class allows us to create a series of points (and normals to those points)
// around a circle or arc of a circle.  Was developed because the same kind of code
// was showing up again and again, and this was a place to consolidate it.

#include "CircleIterator.h"
#include "Global.h"

// =======================================================================================
// Constructor

CircleIterator::CircleIterator(float* centroid, float* dir, float R, float* offset):
                                    arcRadius(R),
                                    center(centroid),
                                    direction(dir)
{
  if(offset)
    glm_vec3_add(centroid, offset, centroid);
  getCrossVectors(center, f1, f2, arcRadius);
}


// =======================================================================================
// Void constructor.  Objects instantiated this way will not be usable until after a call 
// to update();

CircleIterator::CircleIterator(void):
                                center(NULL),
                                direction(NULL)
{
  
}


// =======================================================================================
// Destructor

CircleIterator::~CircleIterator(void)
{
}


// =======================================================================================
// Function to recompute our state when our parameters have changed

void CircleIterator::update(float* centroid, float* dir, float R, float* offset)
{
  bool firstTime = center?false:true;
  center    = centroid;
  direction = dir;
  arcRadius = R;
  if(offset)
    glm_vec3_add(centroid, offset, centroid);
  if(firstTime)
    getCrossVectors(center, f1, f2, arcRadius);
  else
    updateCrossVectors(center, f1, f2, arcRadius);
}


// =======================================================================================
// Function to get a new position (and norm) so many degrees around the circle

void CircleIterator::getPoint(float degrees, float* pos, float* norm)
{
  float angleRadians  = degrees*M_PI/180.0f;

  for(int m=0; m<3; m++)
   {
    norm[m] = cosf(angleRadians)*f1[m] + sinf(angleRadians)*f2[m]; // scaled to radius
    pos[m] = center[m] + norm[m];
   }
}


// =======================================================================================
