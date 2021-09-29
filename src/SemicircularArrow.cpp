// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Render an arrow that follows part of the arc of a circle.  Gets most of the 
// actual implementation functionality from PathTube, and also implements the 
// VisualObject interface, as this will show up in ControlGroups in the Quadtree

#include "SemicircularArrow.h"


// =======================================================================================
/// @brief Constructor
/// @param pos  The center of the circle of which the arrow forms a part of the 
/// circumference
/// @param axis The direction of the axis of the circle above.
/// @param arcD The number of degrees along the circular circumference subtended by the 
/// arrow.
/// @param arcR The radius of the circle above.
/// @param tubeR The radius of the widest part of the tubular body of the arrow. 
/// @param s    The number of sides to use in approximating the tubular rendering of the
/// arrow.

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
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
}


// =======================================================================================
/// @brief Destructor

SemicircularArrow::~SemicircularArrow(void)
{
  delete[] path;
}


// =======================================================================================
/// @brief Function which generates the path used to see pathTube

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
/// @brief Tell callers our name at runtime.
/// @returns A const pointer to the C-string name.

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
/// @brief Function to validate the quadtree and all the objects.
/// @param l The recursive depth in the quadtree/groups that this check is being 
/// conducted at.

void SemicircularArrow::selfValidate(unsigned l)
{
#ifdef LOG_TREE_VALIDATION
   box->selfValidate(true);
#endif
}


// =======================================================================================
