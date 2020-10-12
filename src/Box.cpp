// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// Class for storing and rendering the rectangular boxes (eg straw bales, durisol blocks,
// timber beams, etc).

#include <err.h>
#include "Box.h"

const float heightMarkerSize   = 1.5f; //0.3f; // 1/2 the side of the square in x and y directions
const float heightMarkerHeight = heightMarkerSize*sqrtf(2.0f); // 1/2 the total height

// =======================================================================================
// Constructors.  Note that typically we are constructed dynamically on a pointer, and
// pointers to us are stored both in the Qtree and in the LandSurface

Box::Box(float x, float y, float z)
{
  location[0] = x;
  location[1] = y;
  location[2] = z;
  updateBoundingBox();
}


Box::Box(vec3 loc)
{
  location[0] = loc[0];
  location[1] = loc[1];
  location[2] = loc[2];
  updateBoundingBox();
}


// =======================================================================================
// Destructor

Box::~Box(void)
{
}


// =======================================================================================
// This is where the actual octahedron geometry is defined - we render it into a buffer
// on request

bool Box::bufferGeometry(TriangleBuffer* T)
{
  VertexBufElement* vertices;
  unsigned* indices;
  unsigned vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, 8u, 36u))
    return false;
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
 // vertices[0].set(location[0], location[1],
 //                 location[2]);  //bottom vertex
 
//  if(useNoTexColor)
//    for(int i=0; i<6; i++)
//      vertices[i].setNoTexColor(noTexColor);
  
  // Lower facing south
  //indices[0] = vOffset;
  //indices[1] = vOffset + 2u;
  //indices[2] = vOffset + 1u;


  return true;
}


// =======================================================================================
// How much space we need in a TriangleBuffer

void Box::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 8u;
  iCount = 36u;
}


// =======================================================================================
// Stub definition

void Box::draw(void)
{
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool Box::matchRay(vec3& position, vec3& direction, float& lambda)
{
  if(!box->matchRay(position, direction, lambda))
    return false;
  
  // So it touches our bounding box, have to test the faces.
  
  //XXX NOT DONE
  
  return true;
}


// =======================================================================================
// Compute the bounding box.
                                                                
void Box::updateBoundingBox(void)
{
  if(!box)
    box = new BoundingBox(location[0] - heightMarkerSize, location[1] - heightMarkerSize,
                      location[2], location[0] + heightMarkerSize,
                      location[1] + heightMarkerSize, location[2] + 2.0f*heightMarkerHeight);
  return;
}
                                                                

// =======================================================================================
// Tell callers our name at runtime.

const char* Box::objectName(void)
{
  static char* name = (char*)"Box";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular HeightMarker.

bool Box::diagnosticHTML(HttpDebug* serv)
{
  
  serv->addResponseData("<tr><td>Box</td>");
  serv->respPtr += sprintf(serv->respPtr,
                           "<td><b>Location:</b> (%.1f, %.1f, %.1f)</td></tr>\n",
                           location[0], location[1], location[2]);
  return true;
}


// =======================================================================================
