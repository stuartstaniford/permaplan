// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is a subclass of ObjectGroup specifically to handle the visual controls
// that appear around an object after it has been double-clicked.

#ifndef CONTROL_GROUP_H
#define CONTROL_GROUP_H

#include "ObjectGroup.h"

// =======================================================================================
// Class variable initialization

class ControlGroup: public ObjectGroup
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ControlGroup(VisualObject* firstObject);
  ~ControlGroup(void);
  inline bool bufferGeometry(TriangleBuffer* T)
    {return DisplayList::bufferGeometry(T);}
  inline void adjustAltitudes(LandSurfaceRegion* surface)
    {DisplayList::adjustAltitudes(surface);}
  inline void triangleBufferSizes(unsigned& vCount, unsigned& iCount)
    {DisplayList::triangleBufferSizes(vCount, iCount);}
  const char* objectName(void);
 
private:
  
  // Instance variables - private
  VisualObject* masterObject;
  
  // Member functions - private
  ControlGroup(const ControlGroup&);                 // Prevent copy-construction
  ControlGroup& operator=(const ControlGroup&);      // Prevent assignment

};

#endif




