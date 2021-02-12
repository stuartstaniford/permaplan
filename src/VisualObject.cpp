
// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree

#include "VisualObject.h"


// =======================================================================================
// Constructors

VisualObject::VisualObject(BoundingBox* B):
                                box(B),
                                useNoTexColor(false)
{
}


// =======================================================================================
// Destructor

VisualObject::~VisualObject(void)
{
  if(box)
    delete box;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool VisualObject::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool VisualObject::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

int VisualObject::getNextIndex(bool resetToFirst)
{
  return -1;
}


// =======================================================================================
// Used when this visual object is going to be a visual indicator with a fixed color.
// XX - should this be hived off into a different class, that could use multiple
// inheritance to pull in, so this isn't present in non-visual indicator subclasses?

void VisualObject::setNoTexColor(vec3 color)
{
  glm_vec3_copy(color, noTexColor);
  useNoTexColor = true;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool VisualObject::bufferGeometry(TriangleBuffer* T)
{
  return false;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool VisualObject::matchRay(vec3& position, vec3& direction, float& lambda)
{
  return false;
}


// =======================================================================================
// Compute the bounding box.

void VisualObject::updateBoundingBox(void)
{
  if(!box)
    box = new BoundingBox();
  else
    box->hugeValify();
    
  Vertex v3;
  bool result;
  for(result = getNextUniqueVertex(true, &v3, PositionOnly); result;
                                    result = getNextUniqueVertex(false, &v3, PositionOnly))
   {
    for(int m=0; m<3; m++)
     {
      if(v3.pos[m] < box->lower[m])
        box->lower[m] = v3.pos[m];
      if(v3.pos[m] > box->upper[m])
        box->upper[m] = v3.pos[m];
     }
   }

  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void VisualObject::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

const char* VisualObject::objectName(void)
{
  static char* name = (char*)"VisualObject";
  return name;
}



// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses
// But in general, this class should own a single row in a table, with
// the type of visual object in the first column, and any details provided
// in the second column.

bool VisualObject::diagnosticHTML(HttpDebug* serv)
{
  serv->addResponseData("<tr><td>VisualObject</td>");
  serv->addResponseData("<td>Unsubclassed: something is wrong.</td></tr>\n");

  return false;
}


// =======================================================================================
