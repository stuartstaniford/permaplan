// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class ..... is a template for real classes

#include "SemicircularArrow.h"


// =======================================================================================
// Constructor

SemicircularArrow::SemicircularArrow(vec3 pos, vec3 axis, 
                                     float arcD, float arcR, float tubeR, int s):
                                      VisualObject(true),
                                      circIter(pos, axis, arcR),
                                      arcDegrees(arcD),
                                      tubeRadius(tubeR)
{
  // Things in PathTube;
  sides       = s;
  closedBase  = true;
  closedTop   = true;
  
  box = new BoundingBox();
  box->unitCube(pos); //XX temp
  generatePath();
  PathTube::updateBoundingBox(box, zeroVec);  //XX can this zerovec here be right?
}


// =======================================================================================
// Destructor

SemicircularArrow::~SemicircularArrow(void)
{
  delete[] path;
}


// =======================================================================================
// Function which generates the path used to see pathTube

void SemicircularArrow::generatePath(void)
{  
  NPath = 15;        // needs to be multiple of 4 (and not leas than 8) plus 3.
  path = new vec4[NPath];
  vec3  norm;
  float angleDegrees  = arcDegrees/(NPath-3);

  // First point (center of closed arrow base)
  circIter.getPoint(0.0f, path[0], norm);
  path[0][3] = 0.0f;  //fourth float of vec4 path is radius

  // Second point (very close to first point, first ring of arrow shaft)
  circIter.getPoint(angleDegrees/10.0f, path[1], norm);
  path[1][3] = tubeRadius;
  
  // Loop over the rest of the arrow shaft.
  for(int j=1; j<=3*(NPath-3)/4; j++)
   {
    // Compute the j'th point around 
    circIter.getPoint(j*angleDegrees, path[j+1], norm);
    path[j+1][3] = tubeRadius;    
   }
  
  // Loop over the arrow head.
  for(int j = 3*(NPath-3)/4, k=0; j<NPath-2; j++, k++)
   {
    circIter.getPoint(j*angleDegrees, path[j+2], norm);
    path[j+2][3] = 2.0f*tubeRadius*4*k/(NPath-3); //fourth float of vec4 path is radius
   }
}


// =======================================================================================
// Tell callers our name at runtime.

const char* SemicircularArrow::objectName(void)
{
  static char* name = (char*)"SemicircularArrow";
  return name;
}

// =======================================================================================
// Function to match a ray 

//XX SemicircularArrow:matchRay()

//unless(box->matchRay(position, direction, lambda, offset))
//  return false;

// check PathTube:: matchRay


// =======================================================================================
// Function to validate the quadtree and all the objects.

#ifdef LOG_TREE_VALIDATION

void SemicircularArrow::selfValidate(unsigned l)
{
   box->selfValidate(true);
}

#endif


// =======================================================================================
