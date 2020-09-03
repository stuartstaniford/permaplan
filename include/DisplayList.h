// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree


#ifndef DISPLAY_LIST_H
#define DISPLAY_LIST_H

#include <vector>
#include "VisualObject.h"

// =======================================================================================
// Class variable initialization

//XX Right now this just creates display lists of HeightMarkers, but it should be
// templated so we can create display lists of anything.

class DisplayList: public std::vector<VisualObject*>
{
 public:
  
  // Instance variables - public

  // Member functions - public
  DisplayList(void);
  DisplayList(std::vector<float*> locations);
  ~DisplayList();
  virtual bool bufferGeometry(TriangleBuffer* T);
  virtual void triangleBufferSize(unsigned& vCount, unsigned& iCount);
  virtual bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
};

#endif




