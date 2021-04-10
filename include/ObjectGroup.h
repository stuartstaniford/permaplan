// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is for a group of objects that are all to be displayed and manipulated as
// a unit (and appear in the Quadtree together).

#ifndef OBJECT_GROUP_H
#define OBJECT_GROUP_H

#include "VisualObject.h"
#include "DisplayList.h"

// =======================================================================================
// Class variable initialization

class ObjectGroup: public VisualObject, public DisplayList
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ObjectGroup(VisualObject* firstObject);
  ~ObjectGroup(void);
  void  add(VisualObject* nextObject);
  bool  updateBoundingBox(BoundingBox* B, vec3 offset);
  void  updateBoundingBox(void);
  inline bool bufferGeometry(TriangleBuffer* T)
    {return DisplayList::bufferGeometry(T);}
  inline void adjustAltitudes(LandSurfaceRegion* surface)
    {DisplayList::adjustAltitudes(surface);}
  inline void triangleBufferSizes(unsigned& vCount, unsigned& iCount)
    {DisplayList::triangleBufferSizes(vCount, iCount);}
  virtual bool matchRay(vec3& position, vec3& direction, float& lambda);
  const char* objectName(void);
#ifdef LOG_TREE_VALIDATION
  void        selfValidate(unsigned l);
#endif

  
private:
  
  // Instance variables - private
  
  // Member functions - private
  ObjectGroup(const ObjectGroup&);                 // Prevent copy-construction
  ObjectGroup& operator=(const ObjectGroup&);      // Prevent assignment

};

#endif




