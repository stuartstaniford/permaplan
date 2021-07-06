// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is not used directly, but instead is a superclass for anything that
// might need to be assembled into TriangleBuffers for display on the GPU.
// This class is typically for "parts" of things - ie they don't have their own
// bounding box and can't be directly stored in the Quadtree themselves, but they
// may be elements of larger objects which can be.  Their co-ordinates are expected
// to be in some frame relative to the larger object of which they are part.

#define VISUAL_ELEMENT_IMPLEMENTATION

#include "VisualElement.h"
#include <assert.h>


// =======================================================================================
// Static variables

vec3 zeroVec = {0.0f, 0.0f, 0.0f};

// =======================================================================================
/// @brief Constructor for the VisualElement.
/// 
/// Currently doesn't do anything at this level.

VisualElement::VisualElement(void)
{
}


// =======================================================================================
/// @brief Destructor for the VisualElement.
/// 
/// Currently doesn't do anything at this level.

VisualElement::~VisualElement(void)
{
}


// =======================================================================================
// Method to return the vertices, with each one being only returned a single time, and
// in an order consistent with getNextIndex.  Normals are generally not well defined from
// calling this method and shouldn't be relied on.
// Stub definition this should be overwritten by implementing subclasses


bool VisualElement::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  err(-1, "Invalid call to VisualElement::getNextUniqueVertex");
}


// =======================================================================================
// This returns the vertices, but in an order which each successive group of three defines
// a triangle, normals are expected to be functional, etc.
// Stub definition this should be overwritten by implementing subclasses

bool VisualElement::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  err(-1, "Invalid call to VisualElement::getNextVertex");
}


// =======================================================================================
// This returns the indices, but in an order which each successive group of three defines
// a triangle.
// Stub definition this should be overwritten by implementing subclasses

int VisualElement::getNextIndex(bool resetToFirst)
{
  err(-1, "Invalid call to VisualElement::getNextIndex");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool VisualElement::bufferGeometry(TriangleBuffer* T, vec3 offset)
{
  err(-1, "Called unimplemented superclass VisualElement::bufferGeometry.\n");
}


// =======================================================================================
// Decide if a ray touches us.  This version will work in many cases, but there may
// be more efficient methods possible for specific subclasses.  This will only work
// if the subclass has implemented getNextVertex.

// XX this might return a match on the far side of the element, instead of nearest.

bool VisualElement::matchRayToElement(vec3& position, vec3& direction, float& lambda, 
                                                                                vec3 offset)
{
  unsigned vCount, iCount;
  triangleBufferSizes(vCount, iCount);
  
  Vertex V[3];
  for(int i=0; i<iCount; i+=3)  // Loop over the triangles
   {
    if(i)
      getNextVertex(false, V, PositionOnly);
    else
      getNextVertex(true, V, PositionOnly);
    getNextVertex(false, V+1, PositionOnly);
    getNextVertex(false, V+2, PositionOnly);
    
    if(mollerTrumbore(V[0].pos, V[1].pos, V[2].pos, position, direction, lambda))
      return true;
   }

  return false;
}


// =======================================================================================
// Compute the bounding box.  This version will work in many cases, but there may
// be more efficient methods possible for specific subclasses.  This will only work
// if the subclass has implemented getNextUniqueVertex.

// Note that subclass version of this must also take on the responsibility of notifying
// our qTreeNode if we've changed our extent.

bool VisualElement::updateBoundingBox(BoundingBox* box, vec3 offset)
{
  bool boxChanged = false;
  assert(box);
    
  Vertex v3;
  bool result;
  for(result = getNextUniqueVertex(true, &v3, PositionOnly); result;
                                    result = getNextUniqueVertex(false, &v3, PositionOnly))
   {
    for(int m=0; m<3; m++)
     {
      if(v3.pos[m] + offset[m] < box->lower[m])
       {
        box->lower[m] = v3.pos[m] + offset[m];
        boxChanged = true;
       }
      if(v3.pos[m] + offset[m] > box->upper[m])
       {
        box->upper[m] = v3.pos[m] + offset[m];
        boxChanged = true;
       }
     }
   }

  return boxChanged;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void VisualElement::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  err(-1, "Called unimplemented superclass VisualElement::triangleBufferSizes.\n");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

const char* VisualElement::elementName(void)
{
  static char* name = (char*)"VisualElement";
  return name;
}



// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses
// But in general, this class should own a single row in a table, with
// the type of visual object in the first column, and any details provided
// in the second column.

bool VisualElement::diagnosticHTML(HttpDebug* serv)
{
  err(-1, "Called unimplemented superclass VisualElement::diagnosticHTML.\n");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses
// But in general, this class should own a single row in a table, with
// the type of visual object in the first column, and any details provided
// in the second column.  A link should be provided to whatever URL serves
// diagnosticHTML (above).

bool VisualElement::diagnosticHTMLSummary(HttpDebug* serv)
{
  err(-1, "Called unimplemented superclass VisualElement::diagnosticHTMLSummary.\n");
}


// =======================================================================================
