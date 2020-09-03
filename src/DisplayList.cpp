
// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree

#include "DisplayList.h"
#include "HeightMarker.h"


// =======================================================================================
// Constructors

DisplayList::DisplayList(void): std::vector<VisualObject*>()
{
}

//NB!!! Two constructors!

DisplayList::DisplayList(std::vector<float*> locations): std::vector<VisualObject*>()
{
  //XX Right now this just creates display lists of HeightMarkers, but it should be
  // templated so we can create display lists of anything.
  int i, N = locations.size();
  for(i=0; i<N; i++)
   {
    HeightMarker* H = new HeightMarker(locations[i]);
    push_back(H);
   }

}

// =======================================================================================
// Destructor

DisplayList::~DisplayList(void)
{
  int i, N = size();
  for(i=0; i<N; i++)
    delete at(i);
}


// =======================================================================================
// Puts all the visualobject contents in a buffer

void DisplayList::bufferGeometry(TriangleBuffer* T)
{
  int i, N = size();
  for(i=0; i<N; i++)
    at(i)->bufferGeometry(T);
}


// =======================================================================================
// Returns the size we need for all our contents

void DisplayList::triangleBufferSize(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
  unsigned vObjectSize, iObjectSize;
  int i, N = size();
  for(i=0; i<N; i++)
   {
    at(i)->triangleBufferSize(vObjectSize, iObjectSize);
    vCount += vObjectSize;
    iCount += iObjectSize;
   }
}


// =======================================================================================
// Stub definition.

bool DisplayList::diagnosticHTML(HttpDebug* serv)
{
  serv->addResponseData("<tr><td>DisplayList</td>");
  serv->addResponseData("<td>Unimplemented.</td></tr>\n");

  return false;
}


// =======================================================================================
