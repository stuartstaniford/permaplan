// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is a specialization of VisualElement specifically for any element
// that can be modeled as sweeping a vector path around an axis.  Things like cylinder,
// cone, etc can inherit from this.


#include "AxialElement.h"
#include "CircleIterator.h"
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
// The amount of vertex and index space we would need in a triangle buffer

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
// This is where the actual geometry is defined - we render it into a buffer on request

bool AxialElement::bufferGeometry(TriangleBuffer* T, vec3 offset)
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
  
  //XX this is not testing the closed base or top, so there is a rare case when it fails
  // due to the ray going through the middle of the tube
  
  return false;
}


// =======================================================================================
// Compute the bounding box.

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
// Stub definition this should be overwritten by implementing subclasses

const char* AxialElement::elementName(void)
{
  static char* name = (char*)"AxialElement";
  return name;
}


// =======================================================================================
// Function to print out in JSON format.

#define bufprintf(...) if((buf += snprintf(buf, end-buf,  __VA_ARGS__)) >= end) {return -1;}

int AxialElement::printOPSF(char*& buf, unsigned bufSize)
{
  char* end = buf + bufSize;

  return (bufSize - (int)(end-buf));
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
