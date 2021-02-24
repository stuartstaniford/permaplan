
// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Class that contains a list of visual objects and some methods for managing
// the list as a whole.  Handy for quickly throwing up a set of something
// or others for display.

#include "DisplayList.h"
#include "HeightMarker.h"
#include "LandSurfaceRegion.h"


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

// Note remaining methods do not have any specificity to HeightMarker and should work
// correctly with any VisualObject subclass.

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
   {
    at(i)->bufferGeometry(T);

#ifdef LOG_DISPLAYLIST_BUFFER
    if(strcmp(at(i)->objectName(), (char*)"Arrow") == 0) //XX no arrow bounding box yet.
      continue;
    float centroid[3];
    for(int m=0; m<3; m++)
      centroid[m] = (at(i)->box->lower[m] + at(i)->box->upper[m])/2.0f;
    LogDisplayListBuffer("Buffering %s object at %.1f, %.1f, %.1f.\n",
                         at(i)->objectName(), centroid[0], centroid[1], centroid[2]);
#endif
   }
}


// =======================================================================================
// This gets called whenever the shape of the land has changed, and tells all the visual
// objects in the list to update their idea of their altitude.

void DisplayList::adjustAltitudes(LandSurfaceRegion* surface)
{
  int i, N = size();
  for(i=0; i<N; i++)
    at(i)->setAltitude(surface);
}


// =======================================================================================
// Returns the size we need for all our contents

void DisplayList::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  vCount = 0u;
  iCount = 0u;
  unsigned vObjectSize, iObjectSize;
  int i, N = size();
  for(i=0; i<N; i++)
   {
    at(i)->triangleBufferSizes(vObjectSize, iObjectSize);
    vCount += vObjectSize;
    iCount += iObjectSize;
   }
}


// =======================================================================================
// We just do our kids one at a time, we are transparent ourself

bool DisplayList::diagnosticHTML(HttpDebug* serv)
{
  int i, N = size();
  for(i=0; i<N; i++)
    at(i)->diagnosticHTML(serv);

  return false;
}


// =======================================================================================
