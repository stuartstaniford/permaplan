// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is a specialization of VisualElement specifically for any element
// that can be modeled as sweeping a vector path around an axis.  Things like cylinder,
// cone, etc can inherit from this.


#include "AxialElement.h"
#include "CircleIterator.h"
#include <assert.h>


// =======================================================================================
/// @brief Constructor for an AxialElement.
///
/// Note that this is declared protected, so that it can only be invoked by subclasses.
/// One cannot declare a bare AxialElement.
/// @param root A vector to the base of the axis of the element.
/// @param dir A vector of the length and direction of the axis of the element.
/// @param R The (float) maximum radial extent of the element from its axis at any point along 
/// the axis.
/// @param S An unsigned for the number of sides to use in approximating the sweep of the
/// element around the axis.
/// @todo XX We should somehow make the sides adaptive based on how close to the object
/// we are.

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
/// @brief Destructor - currenty empty.

AxialElement::~AxialElement(void)
{
}


// =======================================================================================
/// @brief Set the length of the AxialElement to a supplied amount.  
/// 
/// This will lengthen or compress the element along it's axis to the specified length.
/// Note we don't do anything about rebuffering - caller must track that.  We also don't 
/// sanity check the new length.
/// @param length The new length of the axis (and element).

void AxialElement::setLength(float length)
{
  float lenNow = glm_vec3_norm(axisDirection);
  glm_vec3_scale(axisDirection, length/lenNow, axisDirection);
}


// =======================================================================================
/// @brief Change the length of the AxialElement by a specified increment.  
/// 
/// This will lengthen or compress the element along it's axis to the newly changed 
/// length.  Note we don't do anything about rebuffering - caller must track that.  We 
/// also don't sanity check the new length.
/// @param increment The amount by which to change the length of the axis (and element).

void AxialElement::lengthen(float increment)
{
  float lenNow = glm_vec3_norm(axisDirection);
  glm_vec3_scale(axisDirection, (lenNow+increment)/lenNow, axisDirection);
}


// =======================================================================================
/// @brief The amount of vertex and index space we would need in a triangle buffer.
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.

void AxialElement::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = sides*NVecs;
  iCount = 6*sides*(NVecs-1);
  
  if(closedBase)
   {
    vCount++;
    iCount += 3*sides;
   }

  if(closedTop)
   {
    vCount++;
    iCount += 3*sides;
   }
  LogTriangleBufEstimates("AxialElement TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
/// @brief This is where the actual geometry is defined - we render it into a buffer on request
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).
/// @param offset A vec3 of the position of this element's containing object, thus 
/// allowing the absolute position of the element to be computed (since we only know our
/// relative position).

bool AxialElement::bufferGeometryOfElement(TriangleBuffer* T, vec3 offset)
{
    float     angleRadians  = 2.0f*M_PI/sides;
    Vertex*   vertices;
    unsigned* indices;
    unsigned  vOffset, vCount, iCount;
    
    triangleBufferSizes(vCount, iCount);
    unless(T->requestSpace(&vertices, &indices, vOffset, vCount, iCount))
     {
      LogTriangleBufferErrs("AxialElement TriangleBuffer request for %u,%u failed at %u.\n",
                                                            vCount, iCount, vOffset);
      return false;
     }
  
    getCrossVectors(axisDirection, f1, f2, radius);

    // Now that we've done some initial setup, we can start looping over the radial slices.
    float ang, cosAng, sinAng;
    vec3 point, norm;
    for(int i=0; i<sides; i++)
     {
      ang = i*angleRadians;
      cosAng = cosf(ang);
      sinAng = sinf(ang);
      for(int m=0; m<3; m++)
        norm[m] = cosAng*f1[m] + sinAng*f2[m]; // scaled to radius
      
      // Now loop over the points in the vertexPath on this slice
      for(int j=0; j<NVecs; j++)
       {
        // this point (j) on this particular radial slice (i)
        for(int m=0; m<3; m++)
          point[m] = location[m] + vectorPath[j][0]*norm[m]
                                        + vectorPath[j][1]*axisDirection[m] + offset[m];
        vertices[NVecs*i+j].setPosition(point);
        vertices[NVecs*i+j].setColor(color);
        
        //XX This is still cylindrical, norm lacks a possible component in the axis direction
        vertices[NVecs*i+j].setNormal(norm); // to be normalized in gpu
        vertices[NVecs*i+j].setObjectId(getObjectIndex());
       }
     }
 
    if(closedBase)
     {
      glm_vec3_scale(axisDirection, -1.0f, norm);
      for(int m=0; m<3; m++)
        point[m] = location[m] + offset[m];
      vertices[NVecs*sides].setPosition(point);
      vertices[NVecs*sides].setNormal(norm);
      vertices[NVecs*sides].setColor(color);
      vertices[NVecs*sides].setObjectId(getObjectIndex());
     }

    if(closedTop)
     {
      for(int m=0; m<3; m++)
        point[m] = location[m] + axisDirection[m] + offset[m];
      vertices[vCount-1].setPosition(point);
      vertices[vCount-1].setNormal(axisDirection);
      vertices[vCount-1].setColor(color);
      vertices[vCount-1].setObjectId(getObjectIndex());

     }

    // Done with vertices, now set up the indices.  As usual, we need triangles to be
    // counter-clockwise looking from outside the element, because of OpenGL faceculling.
    int iPlus, iBase;
    for(int i=0; i<sides; i++)
     {
      iPlus = (i+1)%sides;
      for(int j=0; j<NVecs-1;j++)
       {
        iBase = 6*((NVecs-1)*i + j);
        indices[iBase]    = vOffset + NVecs*i + j;          // base of this radius
        indices[iBase+1]  = vOffset + NVecs*iPlus + j;      // base of shaft at next radius
        indices[iBase+2]  = vOffset + NVecs*i + j + 1;      // top of shaft at this radius
        indices[iBase+3]  = indices[iBase + 1];             // base of shaft at next radius
        indices[iBase+4]  = vOffset + NVecs*iPlus + j + 1;  // top of shaft at next radius
        indices[iBase+5]  = indices[iBase+2];               // top of shaft at this radius
       }
      
      if(closedBase)
       {
        int triBase = 6*sides*(NVecs-1) + 3*i;
        iBase = 6*(NVecs-1)*i;
        indices[triBase]      = vOffset + NVecs*sides;  // bottom center
        indices[triBase + 1]  = indices[iBase + 1];     //
        indices[triBase + 2]  = indices[iBase];         // bottom center
       }
      
      if(closedTop)
       {
        int triBase = iCount - 3*sides + 3*i;
        iBase = 6*((NVecs-1)*i + NVecs-2);
        indices[triBase]      = vOffset + vCount - 1;   // top center
        indices[triBase + 1]  = indices[iBase + 2];     // top of shaft at this radius
        indices[triBase + 2]  = indices[iBase + 4];     // top of shaft at next radius
       }
     }

  return true;
}


// =======================================================================================
/// @brief Decide if a ray touches us.  
/// 
/// Function to decide whether a given line touches the object or not. First checks using
/// a [skew line calculation](https://en.wikipedia.org/wiki/Skew_lines#Distance), whether
/// a match is plausible, and if so does a full check with 
/// AxialElement::matchRayBruteForce
///
/// @returns True if the ray touches us, otherwise false.
/// @param position The vec3 for a point on the ray to be matched.
/// @param direction The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.
/// @param offset A vec3 which gives the position of this element relative to its
/// containing object (since elements generally have relative positions, this is needed
/// to compute absolute position matches).
/// @todo XX This might return a match on the far side of the element, instead of nearest.

bool AxialElement::matchRayToElement(vec3& position, vec3& direction, float& lambda, 
                                                                                vec3 offset)
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
/// @brief This matches every triangle to be certain whether the ray hits or not
/// @todo XX might return a hit on far side of the element, instead of nearest.
/// @todo XX Also this routine causes us to compute every vertex twice.  There might be some
/// clever optimization that could cut down on that.

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
  
  //XX this is not testing the closed base or top, so there is a rare case when it fails
  // due to the ray going through the middle of the tube
  
  return false;
}


// =======================================================================================
/// @brief Updates the bounding box.
/// 
/// Function to update an axis-aligned bounding box.    
/// @todo XX Comment from VisualElement: "Note that subclass version of this must also 
/// take on the responsibility of notifying our qTreeNode if we've changed our extent."
/// This has not been understood or addressed.
/// @returns True if the bounding box was changed in anyway, false otherwise.
/// @param box The bounding box that is to be updated.
/// @param offset The vec3 of our offset from a containing object.

bool AxialElement::updateBoundingBox(BoundingBox* box, vec3 offset)
{
  bool  retVal        = false;
  float angleRadians  = 2.0f*M_PI/sides;
  float ang, cosAng, sinAng;
  vec3 relativeLoc;
  vec3 point;

  glm_vec3_add(location, offset, relativeLoc);
  
  // bottom end point
  if(closedBase)
    if(box->extends(relativeLoc))
      retVal = true;
  
  // top end point
  if(closedTop)
   {
    glm_vec3_add(relativeLoc, axisDirection, point);
    if(box->extends(point))
      retVal = true;
   }

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
        point[m] = relativeLoc[m] + vectorPath[j][0]*(cosAng*f1[m] + sinAng*f2[m])
                          + vectorPath[j][1]*axisDirection[m];
      if(box->extends(point))
        retVal = true;
     }
   }

  return retVal;
}


// =======================================================================================
/// @brief Provide the name of our type of element if requested.

const char* AxialElement::elementName(void)
{
  static char* name = (char*)"AxialElement";
  return name;
}


// =======================================================================================
/// @brief Function to print out in JSON format.

int AxialElement::printOPSF(char*& buf, unsigned bufSize)
{
  char* end = buf + bufSize;

  return (bufSize - (int)(end-buf));
}


// =======================================================================================
