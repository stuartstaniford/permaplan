// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class ..... is a template for real classes

#include "SemicircularArrow.h"
#include "AxialElement.h"       // for crossVectors functions
// =======================================================================================
// Constructor

SemicircularArrow::SemicircularArrow(vec3 pos, vec3 axis, 
                                     float arcD, float arcR, float tubeR, int s):
                                      VisualObject(true),
                                      arcDegrees(arcD),
                                      arcRadius(arcR), 
                                      tubeRadius(tubeR), 
                                      sides(s)
{
  glm_vec3_copy(pos, centroid);
  glm_vec3_copy(axis, axisDirection);
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
  // Initialize the path space in PathTube's variables
  NPath = 15;        // needs to be multiple of 4 (and not leas than 8) plus 3.
  path = new vec4[NPath];

  // initialize local working variables
  float     angleRadians  = 2.0f*M_PI/(NPath-3)*arcDegrees/360.0f;
  float     ang, cosAng, sinAng;  
  vec3      norm, f1, f2, point;

  // f1 and f2 are a basis at right angles to the semicircle axis
  getCrossVectors(axisDirection, f1, f2, arcRadius);  

  // First point (center of closed arrow base)
  
  // Second point (very close to first point, first ring of arrow shaft)
  
  // Loop over the rest of the arrow shaft.
  for(int j=1; j<3*(NPath-3)/4; j++)
   {
    // Compute the j'th point around 
    ang = j*angleRadians;
    cosAng = cosf(ang);
    sinAng = sinf(ang);
    for(int m=0; m<3; m++)
     {
      norm[m] = cosAng*f1[m] + sinAng*f2[m]; // scaled to radius
      point[m] = path[1][m] + norm[m];
     }
    //fourth float of vec4 path is radius
   }
  
  // Loop over the arrow head.
     
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
