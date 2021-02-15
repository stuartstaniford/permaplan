// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class manages a segment of a trunk/branch/stem of a woody plant or tree.  It
// models the heartwood/sapwood/bark, etc.

#include "WoodySegment.h"
#include <err.h>


// =======================================================================================
// Constructors.

WoodySegment::WoodySegment(Species& species, unsigned short treeIndex, float years,
                           vec3 location):
                              TreePart(treeIndex),
                              heartRadius(0.0f),
                              sapThickness(species.initSapThickness),
                              barkThickness(species.initBarkThickness)
{
  vec3 direction;
  direction[0] = 0.0f;
  direction[1] = 0.0f;
  direction[2] = years*species.stemRate;
  cylinder = new Cylinder(location, direction,
                          heartRadius + sapThickness + barkThickness, WOOD_SEG_SIDES);
}


// =======================================================================================
// Destructor

WoodySegment::~WoodySegment(void)
{
  delete cylinder;
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i])
      delete kids[i];
}


// =======================================================================================
// Buffer the vertices/indices for this part.

bool WoodySegment::bufferGeometry(TriangleBuffer* T)
{
  unless(cylinder->bufferGeometry(T))
    return false;
  
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i])
      unless(kids[i]->bufferGeometry(T))
        return false;
  
  return true;
}



// =======================================================================================
// Assess the number of vertices/indices required in a triangle buffer to render us
// and all our children.

void WoodySegment::triangleBufferSizesRecurse(unsigned& vCount, unsigned& iCount)
{
  // Add ourselves
  unsigned V, I;
  cylinder->triangleBufferSizes(V, I);
  vCount += V;
  iCount += I;
  
  // Add our branches, etc
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i])
      kids[i]->triangleBufferSizesRecurse(vCount, iCount);
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
              cylinder->location[0], cylinder->location[1], cylinder->location[2],
              cylinder->direction[0], cylinder->direction[1], cylinder->direction[2]);
  
  int N = kids.size();
  for(int i=0; i<N; i++)
    if(kids[i])
      unless(kids[i]->diagnosticHTML(serv))
        return false;
         
  return true;
}


// =======================================================================================
