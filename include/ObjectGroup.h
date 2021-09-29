// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -

#ifndef OBJECT_GROUP_H
#define OBJECT_GROUP_H

#include "VisualObject.h"
#include "DisplayList.h"


// =======================================================================================
/// @brief Group objects together into an aggregate object. 
/// 
/// This class is for a group of objects that are all to be displayed and manipulated 
/// as a unit (and appear in the Quadtree together).  Mostly it is a superclass for more
/// specific kinds of groups (eg ControlGroup).  It inherits from VisualObject (for
/// interfaces/functionality for being in the Quadtree) and also from DisplayList (for
/// functionality associated with managing a list of VisualObjects).

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
  virtual bool matchRayToObject(vec3& position, vec3& direction, float& lambda);
  const char* objectName(void);
  bool   quadSearchRecursive(HttpDebug* serv, int& nextRow, char* searchTerm, char* quadPath);
  bool        diagnosticHTML(HttpDebug* serv);
  void        selfValidate(unsigned l);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  ObjectGroup(const ObjectGroup&);                 // Prevent copy-construction
  ObjectGroup& operator=(const ObjectGroup&);      // Prevent assignment
};


// =======================================================================================

#endif




