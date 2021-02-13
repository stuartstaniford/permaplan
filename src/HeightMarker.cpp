
// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// Class for storing and rendering the user supplied locations of height markers.
// Renders them as an octahedron balanced on the actual height location

#include <err.h>
#include "HeightMarker.h"

const float heightMarkerSize   = 1.5f; //0.3f; // 1/2 the side of the square in x and y directions
const float heightMarkerHeight = heightMarkerSize*sqrtf(2.0f); // 1/2 the total height

// =======================================================================================
// Constructors.  Note that typically we are constructed dynamically on a pointer, and
// pointers to us are stored both in the Qtree and in the LandSurface

HeightMarker::HeightMarker(float x, float y, float z)
{
  location[0] = x;
  location[1] = y;
  location[2] = z;
  updateBoundingBox();
}


HeightMarker::HeightMarker(vec3 loc)
{
  location[0] = loc[0];
  location[1] = loc[1];
  location[2] = loc[2];
  updateBoundingBox();
}


// =======================================================================================
// Destructor

HeightMarker::~HeightMarker(void)
{
}


// =======================================================================================
//XX Stub definition needs to be implemented

bool HeightMarker::getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
//XX Stub definition needs to be implemented
bool HeightMarker::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  return false;
}


// =======================================================================================
//XX Stub definition needs to be implemented
int HeightMarker::getNextIndex(bool resetToFirst)
{
  return -1;
}


// =======================================================================================
// This is where the actual octahedron geometry is defined - we render it into a buffer
// on request

bool HeightMarker::bufferGeometry(TriangleBuffer* T)
{
  Vertex* vertices;
  unsigned* indices;
  unsigned vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, 6u, 24u))
    return false;
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
  vertices[0].set(location[0], location[1],
                  location[2]);  //bottom vertex
  vertices[1].set(location[0] - heightMarkerSize, location[1] - heightMarkerSize,
                  location[2] + heightMarkerHeight);  // south west corner of center square
  vertices[2].set(location[0] + heightMarkerSize, location[1] - heightMarkerSize,
                  location[2] + heightMarkerHeight);  // south east corner
  vertices[3].set(location[0] + heightMarkerSize, location[1] + heightMarkerSize,
                  location[2] + heightMarkerHeight);  // north east corner
  vertices[4].set(location[0] - heightMarkerSize, location[1] + heightMarkerSize,
                  location[2] + heightMarkerHeight);  // north west corner
  vertices[5].set(location[0], location[1],
                  location[2] + 2.0f*heightMarkerHeight);  //top vertex

  if(useNoTexColor)
    for(int i=0; i<6; i++)
      vertices[i].setNoTexColor(noTexColor);
  
  // Lower facing south
  indices[0] = vOffset;
  indices[1] = vOffset + 2u;
  indices[2] = vOffset + 1u;

  // Lower facing east
  indices[3] = vOffset;
  indices[4] = vOffset + 3u;
  indices[5] = vOffset + 2u;

  // Lower facing north
  indices[6] = vOffset;
  indices[7] = vOffset + 4u;
  indices[8] = vOffset + 3u;

  // Lower facing west
  indices[9] = vOffset;
  indices[10] = vOffset + 1u;
  indices[11] = vOffset + 4u;

  // Upper facing south
  indices[12] = vOffset + 5u;
  indices[13] = vOffset + 1u;
  indices[14] = vOffset + 2u;

  // Upper facing east
  indices[15] = vOffset + 5u;
  indices[16] = vOffset + 2u;
  indices[17] = vOffset + 3u;

  // Upper facing north
  indices[18] = vOffset + 5u;
  indices[19] = vOffset + 3u;
  indices[20] = vOffset + 4u;

  // Upper facing west
  indices[21] = vOffset + 5u;
  indices[22] = vOffset + 4u;
  indices[23] = vOffset + 1u;

  return true;
}


// =======================================================================================
// How much space we need in a TriangleBuffer

void HeightMarker::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 6u;
  iCount = 24u;
}


// =======================================================================================
// Stub definition

void HeightMarker::draw(void)
{
  return;
}


// =======================================================================================
// Stub definition this should be overwritten by implementing subclasses

bool HeightMarker::matchRay(vec3& position, vec3& direction, float& lambda)
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
                                                                
void HeightMarker::updateBoundingBox(void)
{
  if(!box)
    box = new BoundingBox(location[0] - heightMarkerSize, location[1] - heightMarkerSize,
                      location[2], location[0] + heightMarkerSize,
                      location[1] + heightMarkerSize, location[2] + 2.0f*heightMarkerHeight);
  return;
}
                                                                

// =======================================================================================
// Tell callers our name at runtime.

const char* HeightMarker::objectName(void)
{
  static char* name = (char*)"Height Marker";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular HeightMarker.

bool HeightMarker::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("<tr><td>HeightMarker</td>");
  httPrintf("<td><b>Location:</b> (%.1f, %.1f, %.1f)</td></tr>\n",
                                                    location[0], location[1], location[2]);
  return true;
}


// =======================================================================================
