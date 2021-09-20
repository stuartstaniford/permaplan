// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class allows us to create a series of points (and normals to those points)
// around a circle or arc of a circle.  Was developed because the same kind of code
// was showing up again and again, and this was a place to consolidate it.

#include "CircleIterator.h"
#include "Global.h"

// =======================================================================================
/// @brief Constructor
/// @param centroid A pointer to a floating array (with at least three values) for the
/// centroid of the circular arc on which we are to create points.
/// @param dir A pointer to float array with the direction of the axis of the circle.
/// @param R A float for the radius of the circle.
/// @param offset A float* pointer to an optional offset (which would be added to the 
/// centroid if present).  The parameter value is optional and defaults to NULL, which
/// will cause no addition to the centroid.

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
/// @brief Void constructor.
/// Objects instantiated this way will not be usable until after a call to update();

CircleIterator::CircleIterator(void):
                                center(NULL),
                                direction(NULL)
{
  
}


// =======================================================================================
/// @brief Destructor

CircleIterator::~CircleIterator(void)
{
}


// =======================================================================================
/// @brief Function to recompute our state when our parameters have changed.
/// 
/// This is used for example when we are being used to generate points around a series
/// of circles (for example along a tube).
/// @param centroid A pointer to a floating array (with at least three values) for the
/// centroid of the circular arc on which we are to create points.
/// @param dir A pointer to float array with the direction of the axis of the circle.
/// @param R A float for the radius of the circle.
/// @param offset A float* pointer to an optional offset (which would be added to the 
/// centroid if present).  The parameter value is optional and defaults to NULL, which
/// will cause no addition to the centroid.

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
/// @brief Get a new position (and norm) so many degrees around the circle
/// 
/// This is the main method for actually getting the output of the iterator.
/// @param degrees The angular distance aound the circle (in degrees)
/// @param pos A float* to the vector where the newly calculated position should be 
/// stored
/// @param norm A float* to the vector where the newly calculated normal should be stored.

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
