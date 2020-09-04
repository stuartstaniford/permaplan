
// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// Class for storing and rendering the user supplied locations of height markers.
// Renders them as an octahedron balanced on the actual height location

#include <err.h>
#include "Arrow.h"


// =======================================================================================
// Constructors.


Arrow::Arrow(vec3 root, vec3 dir)
{
  glm_vec3_copy(root, location);
  glm_vec3_copy(dir, direction);
  updateBoundingBox();
}


// =======================================================================================
// Destructor

Arrow::~Arrow(void)
{
}


// =======================================================================================
// This is where the actual octahedron geometry is defined - we render it into a buffer
// on request

bool Arrow::bufferGeometry(TriangleBuffer* T)
{
  VertexBufElement* vertices;
  unsigned* indices;
  unsigned vOffset, iOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, iOffset, 0u, 0u))
    return false;
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
  //vertices[0].set(location[0], location[1],
  //                location[2]);  //bottom vertex

  // Lower facing south
  //indices[0] = vOffset;
  //indices[1] = vOffset + 2u;
  //indices[2] = vOffset + 1u;

  return true;
}


// =======================================================================================
// How much space we need in a TriangleBuffer

void Arrow::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
}


// =======================================================================================
// Stub definition

void Arrow::draw(void)
{
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool Arrow::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(!box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
// Bounding box is aligned with the square of the octahedron (which is aligned in the x-y
// plane).  Height markers are currently immutable once created.
                                                                
void Arrow::updateBoundingBox(void)
{
/*  if(!box)
    box = new BoundingBox(location[0] - heightMarkerSize, location[1] - heightMarkerSize,
                      location[2], location[0] + heightMarkerSize,
                      location[1] + heightMarkerSize, location[2] + 2.0f*heightMarkerHeight);
*/
 return;
}
                                                                

// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular HeightMarker.

bool Arrow::diagnosticHTML(HttpDebug* serv)
{
  serv->addResponseData("<tr><td>Arrow</td>");
  serv->respPtr += sprintf(serv->respPtr,
                           "<td><b>Location:</b> (%.1f, %.1f, %.1f)<br>" ,
                           location[0], location[1], location[2]);
  serv->respPtr += sprintf(serv->respPtr,
                           "Direction:</b> (%.1f, %.1f, %.1f)</td></tr>\n" ,
                           direction[0], direction[1], direction[2]);
  return true;
}


// =======================================================================================
