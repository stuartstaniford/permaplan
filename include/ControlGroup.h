// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -

#ifndef CONTROL_GROUP_H
#define CONTROL_GROUP_H

#include "ObjectGroup.h"


// =======================================================================================
/// @brief Subclass of ObjectGroup to handle the visual controls that appear around 
/// an object after it has been double-clicked.
/// 
/// It's capable of manage the arrows, semi-circular arrows, etc, that are 
/// used to move the object around, and the object will be grouped with them.

class ControlGroup: public ObjectGroup
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ControlGroup(VisualObject* firstObject);
  ~ControlGroup(void);
  inline bool bufferGeometryOfObject(TriangleBuffer* T)
    {return DisplayList::bufferGeometry(T);}
  inline void adjustAltitudes(LandSurfaceRegion* surface)
    {DisplayList::adjustAltitudes(surface);}
  inline void triangleBufferSizes(unsigned& vCount, unsigned& iCount)
    {DisplayList::triangleBufferSizes(vCount, iCount);}
  const char* objectName(void);
  void  addOneArrow(vec3 pos, vec3 dir);
  void  createTranslationArrows(void);
  void  createRotationArrows(void);
  bool  diagnosticHTMLSummary(HttpDebug* serv);

private:
  
  // Instance variables - private
  VisualObject* masterObject;
  
  // Member functions - private
  ControlGroup(const ControlGroup&);                 // Prevent copy-construction
  ControlGroup& operator=(const ControlGroup&);      // Prevent assignment

};

// =======================================================================================

#endif




