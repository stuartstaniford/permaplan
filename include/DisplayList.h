// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -

#ifndef DISPLAY_LIST_H
#define DISPLAY_LIST_H

#include <unordered_set>
#include "VisualObject.h"
#include "LandSurfaceRegion.h"


// =======================================================================================
/// @brief Contain for a set of VisualObject.
/// 
/// This class contains a set of visual objects and some methods for managing
/// them as a whole.  Handy for quickly throwing up a set of something or others for 
/// display.
/// @todo Right now this just creates display lists of HeightMarkers when fed a location
/// list in a constructor, but it should be generalized so we can create display lists
/// of anything as the default in that scenario.

class DisplayList: public std::unordered_set<VisualObject*>
{
 public:
  
  // Instance variables - public

  // Member functions - public
  DisplayList(void);
  DisplayList(std::vector<float*> locations);
  ~DisplayList();
  bool bufferGeometry(TriangleBuffer* T);
  void adjustAltitudes(LandSurfaceRegion* surface);
  void triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  DisplayList(const DisplayList&);                 // Prevent copy-construction
  DisplayList& operator=(const DisplayList&);      // Prevent assignment
};

#endif




