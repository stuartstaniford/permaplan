// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class manages a segment of a trunk/branch/stem of a woody plant or tree.  It
// models the heartwood/sapwood/bark, etc.

#include "WoodySegment.h"
#include <err.h>


// =======================================================================================
// Constructors.

WoodySegment::WoodySegment(Species& species, unsigned short treeIndex, float years):
                              TreePart(treeIndex),
                              //length(years*species.stemRate),
                              heartRadius(0.0f),
                              sapThickness(species.initSapThickness),
                              barkThickness(species.initBarkThickness)
{
}


// =======================================================================================
// Destructor

WoodySegment::~WoodySegment(void)
{
}


// =======================================================================================
// Buffer the vertices/indices for this part.

bool WoodySegment::bufferGeometry(TriangleBuffer* T)
{
  //XX TBD
  
  // XX do recursion
  return true;
}



// =======================================================================================
// Assess the number of vertices/indices required in a triangle buffer to render us
// and all our children.

void WoodySegment::triangleBufferSizesRecurse(unsigned& vCount, unsigned& iCount)
{
  vCount += WOOD_SEG_SIDES*3+1;
  iCount += WOOD_SEG_SIDES*9;
  
  // XX do recursion
}


// =======================================================================================
// Tell callers our name at runtime.

const char* WoodySegment::objectName(void)
{
  static char* name = (char*)"WoodySegment";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.

bool WoodySegment::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("<tr><td>WoodySegment</td><td>[%f, %f, %f]</td><td>[%f, %f, %f]</td><tr>",
              location[0], location[1], location[2], direction[0], direction[1], direction[2]);
  
  //XX recurse on children
  return true;
}


// =======================================================================================
