
// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Class that contains a list of visual objects and some methods for managing
// the list as a whole.  Handy for quickly throwing up a set of something
// or others for display.

#include "DisplayList.h"
#include "HeightMarker.h"
#include "LandSurfaceRegion.h"


// =======================================================================================
// Constructors

DisplayList::DisplayList(void): std::unordered_set<VisualObject*>()
{
}

//NB!!! Two constructors!

DisplayList::DisplayList(std::vector<float*> locations): std::unordered_set<VisualObject*>()
{
  //XX Right now this just creates display lists of HeightMarkers, but it should be
  // templated so we can create display lists of anything.
  int i, N = locations.size();
  for(i=0; i<N; i++)
   {
    HeightMarker* H = new HeightMarker(locations[i]);
    emplace(H);
   }
}

// Note remaining methods do not have any specificity to HeightMarker and should work
// correctly with any VisualObject subclass.

// =======================================================================================
// Destructor

DisplayList::~DisplayList(void)
{
  for(VisualObject* V: *this)
    delete V;
}


// =======================================================================================
// Puts all the visualobject contents in a buffer

bool DisplayList::bufferGeometry(TriangleBuffer* T)
{
  for(VisualObject* V: *this)
   {
#ifdef LOG_DISPLAYLIST_BUFFER
    vec3 centroid;
    V->box->getCentroid(centroid);
    LogDisplayListBuffer("Buffering %s object at %.1f, %.1f, %.1f.\n",
                         V->objectName(), centroid[0], centroid[1], centroid[2]);
#endif
    unless(V->bufferGeometry(T))
      return false;
   }
  return true;
}


// =======================================================================================
// This gets called whenever the shape of the land has changed, and tells all the visual
// objects in the list to update their idea of their altitude.

void DisplayList::adjustAltitudes(LandSurfaceRegion* surface)
{
  for(VisualObject* V: *this)
    V->setAltitude(surface);
}


// =======================================================================================
// Returns the size we need for all our contents

void DisplayList::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  unsigned vObjectSize, iObjectSize;
  for(VisualObject* V: *this)
   {
    V->triangleBufferSizes(vObjectSize, iObjectSize);
    vCount += vObjectSize;
    iCount += iObjectSize;
   }
}


// =======================================================================================
// We just do our kids one at a time, we are transparent ourself

bool DisplayList::diagnosticHTML(HttpDebug* serv)
{
  for(VisualObject* V: *this)
    unless(V->diagnosticHTMLSummary(serv))
      return false;

  return true;
}


// =======================================================================================
