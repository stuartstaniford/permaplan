
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

HeightMarker::HeightMarker(float x, float y, float z):
                                              VisualObject(true)
{
  location[0] = x;
  location[1] = y;
  location[2] = z;
  updateBoundingBox();
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
}


HeightMarker::HeightMarker(vec3 loc):
                              VisualObject(true)
{
  location[0] = loc[0];
  location[1] = loc[1];
  location[2] = loc[2];
  updateBoundingBox();
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
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
  err(-1, "Not implemented in HeightMarker::getNextVertex");
  return false;
}


// =======================================================================================
// Return the next vertex in our sequence

bool HeightMarker::getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail)
{
  if(detail > PositionOnly)
    err(-1, "Not implemented in HeightMarker::getNextUniqueVertex");
  
  if(resetToFirst)
    index = 0;
  else
    index++;
    
  switch(index)
   {
    // Lower facing south
    case 0:
      goto V0;
     case 1:
      goto V2;
    case 2:
      goto V1;

    // Lower facing east
    case 3:
      goto V0;
    case 4:
      goto V3;
    case 5:
      goto V2;

    // Lower facing north
    case 6:
      goto V0;
    case 7:
      goto V4;
    case 8:
      goto V3;

    // Lower facing west
    case 9:
      goto V0;
    case 10:
      goto V1;
    case 11:
      goto V4;

    // Upper facing south
    case 12:
      goto V5;
    case 13:
      goto V1;
    case 14:
      goto V2;

    // Upper facing east
    case 15:
      goto V5;
    case 16:
      goto V2;
    case 17:
      goto V3;

    // Upper facing north
    case 18:
      goto V5;
    case 19:
      goto V3;
    case 20:
      goto V4;

    // Upper facing west
    case 21:
      goto V5;
    case 22:
      goto V4;
    case 23:
      goto V1;

    default:
      return false;
   }

V0:
  v->setPosition(location[0], location[1], location[2]);  //bottom vertex
  return true;
V1:
  v->setPosition(location[0] - heightMarkerSize, location[1] - heightMarkerSize,
                  location[2] + heightMarkerHeight);  // south west corner of center square
  return true;
V2:
  v->setPosition(location[0] + heightMarkerSize, location[1] - heightMarkerSize,
                  location[2] + heightMarkerHeight);  // south east corner
  return true;
V3:
  v->setPosition(location[0] + heightMarkerSize, location[1] + heightMarkerSize,
                  location[2] + heightMarkerHeight);  // north east corner
  return true;
V4:
  v->setPosition(location[0] - heightMarkerSize, location[1] + heightMarkerSize,
                  location[2] + heightMarkerHeight);  // north west corner
  return true;
V5:
  v->setPosition(location[0], location[1],
                  location[2] + 2.0f*heightMarkerHeight);  //top vertex
  return true;
}


// =======================================================================================
//XX Stub definition needs to be implemented
int HeightMarker::getNextIndex(bool resetToFirst)
{
  err(-1, "Not implemented in HeightMarker::getNextIndex");
  return -1;
}


// =======================================================================================
// Function to validate the tree.

#ifdef LOG_TREE_VALIDATION

void HeightMarker::selfValidate(unsigned l)
{
  box->selfValidate(true);
}

#endif

// =======================================================================================
// This is where the actual octahedron geometry is defined - we render it into a buffer
// on request

bool HeightMarker::bufferGeometry(TriangleBuffer* T)
{
  Vertex* vertices;
  unsigned* indices;
  unsigned vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, 6u, 24u))
   {
    LogTriangleBufferErrs("HeightMarker TriangleBuffer request for %u,%u failed at %u.\n",
                                                    6u, 24u, vOffset);
    return false;
   }
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
  vertices[0].setPosition(location[0], location[1],
                  location[2]);  //bottom vertex
  vertices[1].setPosition(location[0] - heightMarkerSize, location[1] - heightMarkerSize,
                  location[2] + heightMarkerHeight);  // south west corner of center square
  vertices[2].setPosition(location[0] + heightMarkerSize, location[1] - heightMarkerSize,
                  location[2] + heightMarkerHeight);  // south east corner
  vertices[3].setPosition(location[0] + heightMarkerSize, location[1] + heightMarkerSize,
                  location[2] + heightMarkerHeight);  // north east corner
  vertices[4].setPosition(location[0] - heightMarkerSize, location[1] + heightMarkerSize,
                  location[2] + heightMarkerHeight);  // north west corner
  vertices[5].setPosition(location[0], location[1],
                  location[2] + 2.0f*heightMarkerHeight);  //top vertex

  if(useNoTexColor)
    for(int i=0; i<6; i++)
      vertices[i].setColor(noTexColor);
  
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
  LogTriangleBufEstimates("HeightMarker TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
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

bool HeightMarker::diagnosticHTMLSummary(HttpDebug* serv)
{
  httPrintf("<tr><td><a href=\"/object/%d\">HeightMarker</a></td>", objIndex);
  httPrintf("<td><b>Location:</b> (%.1f, %.1f, %.1f)</td></tr>\n",
                                                    location[0], location[1], location[2]);
  return true;
}


// =======================================================================================
