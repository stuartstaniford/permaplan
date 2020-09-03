
// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree

#include "VisualObject.h"


// =======================================================================================
// Constructors

VisualObject::VisualObject(BoundingBox* B): box(B)
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

bool VisualObject::bufferGeometry(TriangleBuffer* T)
{
  return false;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void VisualObject::draw(void)
{
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool VisualObject::matchRay(vec3& position, vec3& direction, float& lambda)
{
  return false;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

void VisualObject::updateBoundingBox(void)
{
  if(box)
   {
    delete box;
    box = NULL;
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
