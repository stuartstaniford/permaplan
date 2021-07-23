
// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// Class for storing and rendering the user supplied positions of height markers.
// Renders them as an octahedron balanced on the actual height position

#include "HeightMarker.h"
#include <err.h>

// =======================================================================================
// Useful constants.

const float heightMarkerSize   = 1.5f; //0.3f; // 1/2 the side of the square in x and y directions
const float heightMarkerHeight = heightMarkerSize*sqrtf(2.0f); // 1/2 the total height

#define HM_VCOUNT 6u
#define HM_ICOUNT 24u

// =======================================================================================
/// @brief Constructor.  
/// 
/// Note that typically we are constructed dynamically on a pointer, and pointers to us 
/// are stored both in the Qtree and in the LandSurface.
/// @param x Position of the height to be marked in the x direction.
/// @param y Position of the height to be marked in the y direction.
/// @param z Position of the height to be marked in the z direction.

HeightMarker::HeightMarker(float x, float y, float z):
                                              VisualObject(true),
                                              Positionable(x,y,z)
{
  updateBoundingBox();
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
}


// =======================================================================================
/// @brief Constructor.  
/// 
/// Note that typically we are constructed dynamically on a pointer, and pointers to us 
/// are stored both in the Qtree and in the LandSurface.
/// @param loc vec3 position of the height to be marked.

HeightMarker::HeightMarker(vec3 loc):
                              VisualObject(true),
                              Positionable(loc)
{
  updateBoundingBox();
  LogObjectCreation("Object created of type %s with id %u.\n", objectName(), objIndex);
}


// =======================================================================================
// Destructor

HeightMarker::~HeightMarker(void)
{
}


// =======================================================================================
/// @brief Return the next vertex in our sequence

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
  v->setPosition(position[0], position[1], position[2]);  //bottom vertex
  return true;
V1:
  v->setPosition(position[0] - heightMarkerSize, position[1] - heightMarkerSize,
                  position[2] + heightMarkerHeight);  // south west corner of center square
  return true;
V2:
  v->setPosition(position[0] + heightMarkerSize, position[1] - heightMarkerSize,
                  position[2] + heightMarkerHeight);  // south east corner
  return true;
V3:
  v->setPosition(position[0] + heightMarkerSize, position[1] + heightMarkerSize,
                  position[2] + heightMarkerHeight);  // north east corner
  return true;
V4:
  v->setPosition(position[0] - heightMarkerSize, position[1] + heightMarkerSize,
                  position[2] + heightMarkerHeight);  // north west corner
  return true;
V5:
  v->setPosition(position[0], position[1],
                  position[2] + 2.0f*heightMarkerHeight);  //top vertex
  return true;
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

bool HeightMarker::bufferGeometryOfObject(TriangleBuffer* T)
{
  Vertex* vertices;
  unsigned* indices;
  unsigned vOffset;
  
  unless(T->requestSpace(&vertices, &indices, vOffset, HM_VCOUNT, HM_ICOUNT))
   {
    LogTriangleBufferErrs("HeightMarker TriangleBuffer request for %u,%u failed at %u.\n",
                                                    HM_VCOUNT, HM_ICOUNT, vOffset);
    return false;
   }
  
  // Now we know where we are putting stuff and that there is space, so pack
  // in the vertices
  vertices[0].setPosition(position[0], position[1],
                  position[2]);  //bottom vertex
  vertices[1].setPosition(position[0] - heightMarkerSize, position[1] - heightMarkerSize,
                  position[2] + heightMarkerHeight);  // south west corner of center square
  vertices[2].setPosition(position[0] + heightMarkerSize, position[1] - heightMarkerSize,
                  position[2] + heightMarkerHeight);  // south east corner
  vertices[3].setPosition(position[0] + heightMarkerSize, position[1] + heightMarkerSize,
                  position[2] + heightMarkerHeight);  // north east corner
  vertices[4].setPosition(position[0] - heightMarkerSize, position[1] + heightMarkerSize,
                  position[2] + heightMarkerHeight);  // north west corner
  vertices[5].setPosition(position[0], position[1],
                  position[2] + 2.0f*heightMarkerHeight);  //top vertex

  if(useNoTexColor)
    for(int i=0; i<HM_VCOUNT; i++)
      vertices[i].setColor(noTexColor);
  for(int i=0; i<HM_VCOUNT; i++)
    vertices[i].setObjectId(objIndex);

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
  vCount = HM_VCOUNT;
  iCount = HM_ICOUNT; 
  LogTriangleBufEstimates("HeightMarker TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
// Bounding box is aligned with the square of the octahedron (which is aligned in the x-y
// plane).  Height markers are currently immutable once created.
                                                                
void HeightMarker::updateBoundingBox(void)
{
  if(!box)
    box = new BoundingBox(position[0] - heightMarkerSize, position[1] - heightMarkerSize,
                      position[2], position[0] + heightMarkerSize,
                      position[1] + heightMarkerSize, position[2] + 2.0f*heightMarkerHeight);
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
  httPrintf("<td><b>position:</b> (%.1f, %.1f, %.1f)</td></tr>\n",
                                                    position[0], position[1], position[2]);
  return true;
}


// =======================================================================================
// Build a page about this particular object with all available detail.

bool HeightMarker::diagnosticHTML(HttpDebug* serv)
{
  // Page header
  char title[64];
  snprintf(title, 63, "Detail Page for Heightmarker (Object %d).", objIndex);
  unless(serv->startResponsePage(title))
    return false;
  
  // Summary Data about the marker
  httPrintf("<b>position:</b> (%.1f, %.1f, %.1f)<br>\n",
                                        position[0], position[1], position[2]);
  unless(box->diagnosticHTML(serv))
    return false;
  
  // Page closing
  unless(serv->endResponsePage())
    return false;
  return true;
}


// =======================================================================================
