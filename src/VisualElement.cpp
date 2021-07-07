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
/// @brief Interface for getting the vertices of the object one at a time.
///
/// This interface is for a function that will cycle through all the vertices of all 
/// the triangles of the object.  A given corner may be touched multiple times where
/// multiple triangles meet there.  
/// 
/// The definition in this class is a stub that should be overwritten by implementing 
/// subclasses as this version will abort with an error.  It is not absolutely necessary 
/// to implement it but the generic version of matchRay() relies on it, so a class that
/// doesn't implement this must create a custom implementation of that.

/// @param resetToFirst A boolean that if true says restart the vertex list from the beginning.
/// @param v A point to the vertex to be filled out.
/// @param detail A VertexDetail enum (see Vertex.h) as to whether to include texture and 
/// normal coordinates in addition to the position.

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
/// @brief This defines the interface for any visual element to turn itself into a set of 
/// vertices/indices for rendering.
///
/// The actual definition in this class is a stub that should be overwritten by 
/// implementing subclasses.  This version will abort with an error if ever called.
/// 
/// @returns False if space cannot be obtained in the TriangleBuffer, true otherwise.
/// @param T A pointer to a TriangleBuffer into which the object should insert its
/// vertices and indices (see TriangleBuffer::requestSpace).
/// @param offset A vec3 of the position of this element relative to it's containing
/// object, thus allowing it's absolute position to be computed.

bool VisualElement::bufferGeometryOfElement(TriangleBuffer* T, vec3 offset)
{
  err(-1, "Called unimplemented superclass VisualElement::bufferGeometryOfElement.\n");
}


// =======================================================================================
/// @brief Decide if a ray touches us.  
/// 
/// Function to decide whether a given line touches the object or not.  It is extensively 
/// used for things like identifying the object at mouse position, deciding if light 
/// hits trees from particular angles, etc, etc.  The version here will work in many 
/// cases, but there may be more efficient methods possible for specific subclasses.  
/// This will only work if the subclass has implemented getNextVertex - it brute force 
/// iterates through all the triangles in the element and tests each one using 
/// mollerTrombore.
///
/// @param position The vec3 for a point on the ray to be matched.
/// @param direction The vec3 for the direction of the ray.
/// @param lambda A reference to a float which will be used to store the multiple of 
/// the direction vector from the position to the match point on the object.
/// @param offset A vec3 which gives the position of this element relative to its
/// containing object (since elements generally have relative positions, this is needed
/// to compute absolute position matches).

/// @todo XX This might return a match on the far side of the element, instead of nearest.

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
/// @brief Update a supplied bounding box.  
/// 
/// Function to update an axis-aligned bounding box.  This version will work in many 
/// cases as long as the subclass has implemented getNextUniqueVertex but there may be 
/// more efficient methods possible for specific subclasses.  Note that subclass version 
/// of this must also take on the responsibility of notifying our qTreeNode if we've 
/// changed our extent.
///
/// @returns True if the bounding box was changed in anyway, false otherwise.
/// @param box The bounding box that is to be updated.
/// @param offset The vec3 of our offset from a containing object.

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
/// @brief The interface by which any VisualElement (or VisualObject) can be queried as 
/// to the amount of space it will require in a TriangleBuffer (in a call to 
/// bufferGeometryToElement or bufferGeometryToObject).
/// 
/// The implementation in this class is a stub that should be overwritten by 
/// implementing subclasses - it will abort if ever called.
///
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.


void VisualElement::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  err(-1, "Called unimplemented superclass VisualElement::triangleBufferSizes.\n");
}


// =======================================================================================
/// @brief An interface by which any VisualElement should return a pointer to its name.
///
/// Should be overriden - this one will abort.

const char* VisualElement::elementName(void)
{
  err(-1, "Called unimplemented superclass VisualElement::elementName.\n");
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
