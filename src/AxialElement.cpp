// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is a specialization of VisualElement specifically for any element
// that can be modeled as sweeping a vector path around an axis.  Things like cylinder,
// cone, etc can inherit from this.


#include "AxialElement.h"
#include <assert.h>

// =======================================================================================
// Constructors

AxialElement::AxialElement(vec3 root, vec3 dir, float R, unsigned S):
                                                  radius(R),
                                                  sides(S),
                                                  closedTop(false),
                                                  closedBase(false),
                                                  NVecs(0u),        // subclasses need to set
                                                  vectorPath(NULL)  // ditto
{
  glm_vec3_copy(root, location);
  glm_vec3_copy(dir, axisDirection);
}


// =======================================================================================
// Destructor

AxialElement::~AxialElement(void)
{
}


// =======================================================================================
// Method to return the vertices, with each one being only returned a single time, and
// in an order consistent with getNextIndex.  Normals are generally not well defined from
// calling this method and shouldn't be relied on.
// Stub definition this should be overwritten by implementing subclasses


bool AxialElement::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  err(-1, "Invalid call to AxialElement::getNextUniqueVertex");
}


// =======================================================================================
// Increase the length of the AxialElement by a length increment.  Note we don't do anything
// about rebuffering - caller must track that.

void AxialElement::lengthen(float increment)
{
  float lenNow = glm_vec3_norm(axisDirection);
  glm_vec3_scale(axisDirection, (lenNow+increment)/lenNow, axisDirection);
}


// =======================================================================================
// Set the length of the AxialElement to a supplied amount.  Note we don't do anything
// about rebuffering - caller must track that.

void AxialElement::setLength(float length)
{
  float lenNow = glm_vec3_norm(axisDirection);
  glm_vec3_scale(axisDirection, length/lenNow, axisDirection);
}


// =======================================================================================
// This returns the vertices, but in an order which each successive group of three defines
// a triangle, normals are expected to be functional, etc.
// Stub definition this should be overwritten by implementing subclasses

bool AxialElement::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  err(-1, "Invalid call to AxialElement::getNextVertex");
}


// =======================================================================================
// This returns the indices, but in an order which each successive group of three defines
// a triangle.
// Stub definition this should be overwritten by implementing subclasses

int AxialElement::getNextIndex(bool resetToFirst)
{
  err(-1, "Invalid call to AxialElement::getNextIndex");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool AxialElement::bufferGeometry(TriangleBuffer* T, vec3 offset)
{
  err(-1, "Called unimplemented superclass AxialElement::bufferGeometry.\n");
}


// =======================================================================================
// Figure out whether a ray intersects the element or not
// https://en.wikipedia.org/wiki/Skew_lines#Distance

bool AxialElement::matchRay(vec3& position, vec3& direction, float& lambda, vec3 offset)
{
#ifndef LOG_TREE_MATCH_RAY
  vec3 joinLine, originDiff;
#endif
  vec3 relativePos;

  glm_vec3_sub(position, offset, relativePos);
  glm_vec3_crossn(direction, axisDirection, joinLine);
  glm_vec3_sub(relativePos, location, originDiff);
  float dist = fabs(glm_vec3_dot(joinLine, originDiff));
#ifdef LOG_TREE_MATCH_RAY
  lastRayMatch = dist;
#endif

  if(dist <= radius)
    return matchRayBruteForce(position, direction, lambda, offset);
  else
    return false;
}


// =======================================================================================
// This matches every triangle to be certain whether the ray hits or not
// XX might return a hit on far side of the element, instead of nearest.
// XX Also this routine causes us to compute every vertex twice.  There might be some
// clever optimization that could cut down on that.
bool AxialElement::matchRayBruteForce(vec3& position, vec3& direction, float& lambda,
                                                                                vec3 offset)
{
  float     angleRadians  = 2.0f*M_PI/sides;
  vec3      triangle[3];
  
  getCrossVectors(axisDirection, f1, f2, radius);

  // Now that we've done some initial setup, we can compute all the vertices.
  float ang, cosAng, sinAng;
  for(int i=0; i<sides; i++)
   {
    ang = i*angleRadians;
    cosAng = cosf(ang);
    sinAng = sinf(ang);
    vec3 norm = {cosAng*f1[0] + sinAng*f2[0],
                    cosAng*f1[1] + sinAng*f2[1],
                    cosAng*f1[2] + sinAng*f2[2]}; // note this is scaled to radius
    
    // Now work over all the points in the vectorPath
    for(int j=0; j<NVecs-1;j++)
     {
      // The point right here on this slice, this vectorPath
      for(int m=0; m<3;m++)
        triangle[0][m] = location[m] + vectorPath[j][0]*norm[m]
                                        + vectorPath[j][1]*axisDirection[m] + offset[m];
 
      // Next point up this radial slice (ie j+1)
      for(int m=0; m<3;m++)
        triangle[1][m] = location[m] + vectorPath[j+1][0]*norm[m]
                                        + vectorPath[j+1][1]*axisDirection[m] + offset[m];
      
      // Setup to work with points from next radial slice
      ang = ((i+1)%sides)*angleRadians;
      cosAng = cosf(ang);
      sinAng = sinf(ang);
      for(int m=0; m<3; m++)
        norm[m] = cosAng*f1[m] + sinAng*f2[m];

      // Now complete the triangle with base of next radial slice i+1, but j still
      for(int m=0; m<3; m++)
        triangle[2][m] = location[m] + vectorPath[j][0]*norm[m]
                                      + vectorPath[j][1]*axisDirection[m] + offset[m];

      // test the triangle
      if(mollerTrumbore(triangle, position, direction, lambda))
        return true;

      // Now move up on next radial slice to j+1
      for(int m=0; m<3; m++)
        triangle[2][m] = location[m] + vectorPath[j+1][0]*norm[m]
                                        + vectorPath[j+1][1]*axisDirection[m] + offset[m];

      // ok test again
      if(mollerTrumbore(triangle, position, direction, lambda))
        return true;
     }
   }
  
  return false;
}


// =======================================================================================
// Compute the bounding box.

bool AxialElement::updateBoundingBox(BoundingBox* box, vec3 offset)
{
  bool  retVal        = false;
  float angleRadians  = 2.0f*M_PI/sides;
  float ang, cosAng, sinAng;
  vec3 point;

  // bottom end point
  if(closedBase)
    if(box->extends(location))
      retVal = true;
  
  // top end point
  if(closedTop)
    glm_vec3_add(location, axisDirection, point);
      if(box->extends(point))
        retVal = true;

  getCrossVectors(axisDirection, f1, f2, radius);
  
  for(int i=0; i<sides; i++)
   {
    // Do the setup for this radial slice
    ang = i*angleRadians;
    cosAng = cosf(ang);
    sinAng = sinf(ang);

    // Now do each point in the vectorPath
    for(int j=0; j<NVecs; j++)
     {
      for(int m=0; m<3; m++)
        point[m] = location[m] + vectorPath[j][0]*(cosAng*f1[m] + sinAng*f2[m])
                          + vectorPath[j][1]*axisDirection[m] +  + offset[m];
      if(box->extends(point))
        retVal = true;
     }
   }

  return retVal;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void AxialElement::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = sides*NVecs;
  iCount = 2*sides*(NVecs-1);
  
  if(closedBase)
   {
    vCount++;
    iCount += sides;
   }

  if(closedTop)
   {
    vCount++;
    iCount += sides;
   }
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

const char* AxialElement::elementName(void)
{
  static char* name = (char*)"AxialElement";
  return name;
}



// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses
// But in general, this class should own a single row in a table, with
// the type of visual object in the first column, and any details provided
// in the second column.

bool AxialElement::diagnosticHTML(HttpDebug* serv)
{
  err(-1, "Called unimplemented superclass AxialElement::diagnosticHTML.\n");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses
// But in general, this class should own a single row in a table, with
// the type of visual object in the first column, and any details provided
// in the second column.  A link should be provided to whatever URL serves
// diagnosticHTML (above).

bool AxialElement::diagnosticHTMLSummary(HttpDebug* serv)
{
  err(-1, "Called unimplemented superclass AxialElement::diagnosticHTMLSummary.\n");
}

// =======================================================================================
// Utility function to get two vectors perpendicular to a specified one.
// Eg if sides == 8 the cross section of the TruncatedCone which points in the dir
// direction looks like
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

  if(dir[0] < EPSILON && dir[1] < EPSILON)
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
