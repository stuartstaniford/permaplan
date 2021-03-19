// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is a specialization of VisualElement specifically for any element
// that can be modeled as sweeping a vector path around an axis.  Things like cylinder,
// cone, etc can inherit from this.


#include "AxialElement.h"
#include <assert.h>

// =======================================================================================
// Constructors

AxialElement::AxialElement(vec3 root, vec3 dir, unsigned S):
                                                  sides(S)
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
// Decide if a ray touches us.

bool AxialElement::matchRay(vec3& position, vec3& direction, float& lambda, vec3 offset)
{
  return false;
}


// =======================================================================================
// Compute the bounding box.

bool AxialElement::updateBoundingBox(BoundingBox* box, vec3 offset)
{
  return false;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void AxialElement::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  err(-1, "Called unimplemented superclass VisualElement::triangleBufferSizes.\n");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

const char* AxialElement::elementName(void)
{
  static char* name = (char*)"VisualElement";
  return name;
}



// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses
// But in general, this class should own a single row in a table, with
// the type of visual object in the first column, and any details provided
// in the second column.

bool AxialElement::diagnosticHTML(HttpDebug* serv)
{
  err(-1, "Called unimplemented superclass VisualElement::diagnosticHTML.\n");
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses
// But in general, this class should own a single row in a table, with
// the type of visual object in the first column, and any details provided
// in the second column.  A link should be provided to whatever URL serves
// diagnosticHTML (above).

bool AxialElement::diagnosticHTMLSummary(HttpDebug* serv)
{
  err(-1, "Called unimplemented superclass VisualElement::diagnosticHTMLSummary.\n");
}


// =======================================================================================
