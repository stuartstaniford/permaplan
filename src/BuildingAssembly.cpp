// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// Functionality common to building sub-assemblies such as gables, sheds, etc.  At the 
// moment, has functionality for managing arrays of BuildingRects, along with some
// Rectextensions

#include "BuildingAssembly.h"
#include "BuildingRect.h"


// =======================================================================================
/// @brief Constructor
/// @param N the number of BuildingRect elements in this particular assembly

BuildingAssembly::BuildingAssembly(int nRectangles, int nExtensions):
                                  nRects(nRectangles),
                                  nExts(nExtensions)
{
  if(nRects)
    rects = new BuildingRect[nRects];
  else
    rects = NULL;
  if(nExts)
    exts = new RectExtension[nExts];
  else
    exts = NULL;
}


// =======================================================================================
/// @brief Destructor

BuildingAssembly::~BuildingAssembly(void)
{
  delete[] rects;
  delete[] exts;
}


// =======================================================================================
/// @brief How much space we need in a TriangleBuffer on a call to bufferGeometryToObject
///
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.
/// @todo Stub only at present

void BuildingAssembly::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  rects[0].triangleBufferSizes(vCount, iCount);
  vCount = nRects*vCount + nExts;  // nRects walls plus a vertex for each end cap triangle
  iCount = nRects*iCount + 3*nExts;  // nRects walls plus nExts end cap triangles
  LogTriangleBufEstimates("BuildingAssembly TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
/// @brief This buffers the geometry of the component parts of the assembly
/// /// 
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).

bool BuildingAssembly::bufferGeometryOfObject(TriangleBuffer* T)
{
  vec3 offset;
  getPosition(offset);
  
  for(int i=0; i<nRects; i++)
    unless(rects[i].bufferGeometryOfElement(T, offset))
      return false;

  bufferExtensions(T);
  
  return true;
}


// =======================================================================================
/// @brief After buffering all the BuildingRect pieces, add the triangles defined by
/// the various RectExtensions.
/// 
/// This has to do some tricky buffer math.  Only C programmers allowed in this function 
/// :-). In general, each extension takes two vertices from an existing rectangle and
/// adds one new vertex to form a triangle which is then buffered. 
/// BuildingRect::bufferGeometryOfElement to figure out which vertex is which.  
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).

bool BuildingAssembly::bufferExtensions(TriangleBuffer* T)
{
  // Get the space for the extra triangles
  Vertex*   vertices;
  unsigned* indices;
  unsigned  vOffset;
  unless(T->requestSpace(&vertices, &indices, vOffset, nExts, 3*nExts))
   {
    LogTriangleBufferErrs("BuildingAssembly::bufferExtensions TriangleBuffer request"
                    " for %u, %u failed at %u.\n", nExts, 3*nExts, vOffset);
    return false;
   }
  
  // Figure out the beginning of the rectVertices
  Vertex*   rectVertices;
  unsigned rectVCount, rectICount;
  rects->triangleBufferSizes(rectVCount, rectICount);
  rectVertices = vertices - nRects*rectVCount;
  unsigned rectOffset = vOffset - nRects*rectVCount;
  
  // Now process the RectExtensions one at a time
  for(int i=0; i<nExts; i++)
   {
    // Copy the zeroth vertex from the correct rectangle as the starting point, which
    // will give us the correct norm, color, etc.
    memcpy(vertices+i, rectVertices + (exts[i].rectIndex)*rectVCount, sizeof(Vertex));
    vec3 pt;
    getPosition(pt);
    glm_vec3_add(pt, exts[i].extensionPoint, pt);
    vertices[i].setPosition(pt);
    LogBuildRectDetails("Vertex 4 at [%.1f, %.1f, %.1f].\n", pt[0], pt[1], pt[2]);
    vertices[i].setObjectId(objIndex);    

    // Now indices of the new triangle
    indices[3*i]    = rectOffset + (exts[i].rectIndex)*rectVCount 
                                            + exts[i].vertexIndices[0]; // 1st existing
    indices[3*i+1]  = rectOffset + (exts[i].rectIndex)*rectVCount 
                                            + exts[i].vertexIndices[1];  // 2nd existing
    indices[3*i+2] = vOffset+i;     // newly created vertex
   }
  
  return true;
}


// =======================================================================================
/// @brief Update the size of our axis-aligned bounding box.
///
/// We call each of our component BuildingRects and have them update our bounding box
/// with our position as their offset.
/// @todo - how to handle our orientation?

void BuildingAssembly::updateBoundingBox(void)
{
  unless(box)
    box = new BoundingBox();
  vec3 offset;
  getPosition(offset);
  for(int i=0; i<nRects; i++)
    rects[i].updateBoundingBox(box, offset);
}


// =======================================================================================
/// @brief Get our position vector - has to be supplied by subclass.
///
/// @returns A float* which points to the vec3 of our position.

void BuildingAssembly::getPosition(vec3 result)
{
  err(-1, "Call to non-implemented BuildingAssembly::getPosition.\n");  
}


// =======================================================================================
/// @brief Decide if a ray touches us.
/// 
/// This implementation works by checking each of our component BuildingRects
/// @param pos The vec3 for a point on the ray to be matched.
/// @param dir The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.
/// @todo End triangles

bool BuildingAssembly::matchRayToObject(vec3& pos, vec3& dir, float& lambda)
{
  lambda = HUGE_VAL;
  float subLambda;
  bool matched = false;
  vec3 offset;
  getPosition(offset);  
  
  for(int i=0; i<nRects; i++)
    if(rects[i].matchRayToElement(pos, dir, subLambda, offset))
     {
      matched = true;
      if(subLambda < lambda)
        lambda = subLambda;
     }
  
  return matched;
}


// =======================================================================================
/// @brief Function to validate the quadtree and all the objects.

void BuildingAssembly::selfValidate(unsigned l)
{
#ifdef LOG_TREE_VALIDATION
  box->selfValidate(true);
#endif
}


// =======================================================================================
/// @brief Tell callers our name at runtime.

const char* BuildingAssembly::objectName(void)
{
  static char* name = (char*)"BuildingAssembly";
  return name;
}


// =======================================================================================
