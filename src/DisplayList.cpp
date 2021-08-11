// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Class that contains a list of visual objects and some methods for managing
// the list as a whole.  Handy for quickly throwing up a set of something
// or others for display.

#include "DisplayList.h"
#include "HeightMarker.h"
#include "LandSurfaceRegion.h"


// =======================================================================================
// @brief Constructor for an empty DisplayList

DisplayList::DisplayList(void): std::unordered_set<VisualObject*>()
{
}

//NB!!! Two constructors!

// =======================================================================================
/// @brief Constructor for a display set of objects at a list of supplied locations.
/// @param locations A std::vector<float*> pointing to vec3s where the objects should 
/// be put.
/// @todo Right now this just creates display lists of HeightMarkers, but it should 
/// be generalized so we can create display lists of anything.  Note other methods do 
/// not have any specificity to HeightMarker and should work correctly with any 
/// VisualObject subclass.

DisplayList::DisplayList(std::vector<float*> locations): std::unordered_set<VisualObject*>()
{
  int i, N = locations.size();
  for(i=0; i<N; i++)
   {
    HeightMarker* H = new HeightMarker(locations[i]);
    emplace(H);
   }
}


// =======================================================================================
/// @brief Destructor

DisplayList::~DisplayList(void)
{
  for(VisualObject* V: *this)
    delete V;
}


// =======================================================================================
/// @brief Puts all the visualobject contents in a buffer
/// @returns True if all went well, false if there wasn't enough space.
/// @param T Pointer to the TriangleBuffer our objects should be put in.

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
    unless(V->bufferGeometryOfObject(T))
      return false;
   }
  return true;
}


// =======================================================================================
/// @brief - tell all the visual objects to update their altitude.
/// 
/// This gets called whenever the shape of the land has changed, and tells all the 
/// visual objects in the list to update their idea of their altitude.
/// @param surface Pointer to the LandSurfaceRegion that is asoociated with our objects.

void DisplayList::adjustAltitudes(LandSurfaceRegion* surface)
{
  for(VisualObject* V: *this)
    V->setAltitude(surface);
}


// =======================================================================================
/// @brief Returns the size we need for all our contents
/// @param vCount A reference to a count which will hold the number of Vertex objects 
/// that will be generated.
/// @param iCount A reference to a count which will hold the number of unsigned indices 
/// that will be generated.

void DisplayList::triangleBufferSizes(unsigned& vCount, unsigned& iCount)
{
  unsigned vObjectSize, iObjectSize;
  for(VisualObject* V: *this)
   {
    V->triangleBufferSizes(vObjectSize, iObjectSize);
    vCount += vObjectSize;
    iCount += iObjectSize;
   }
  LogTriangleBufEstimates("DisplayList TriangleBuffer estimate: [%u, %u]\n", vCount, iCount);
}


// =======================================================================================
/// @brief Provides access to the diagnostic HTTP server for the diagnosticHTML page.
///
/// We just do our kids one at a time, we are transparent ourself
/// @returns True if the HeightMarker was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool DisplayList::diagnosticHTML(HttpDebug* serv)
{
  for(VisualObject* V: *this)
    unless(V->diagnosticHTMLSummary(serv))
      return false;

  return true;
}


// =======================================================================================
