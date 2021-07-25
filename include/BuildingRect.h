// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef BUILDING_RECT_H
#define BUILDING_RECT_H

#include "VisualElement.h"


// =======================================================================================
// Required forward declarations

class Gable;
class Shed;


// =======================================================================================
/// @brief Class for the plain old data in a BuildingRect (so it can be memcpied).

class BuildRectData
{
 protected:
  vec3 sides[2];
  vec3 relativePos;
  unsigned color;
  bool normForward;  // normal is in the direction of sides[0]xsides[1]
};


// =======================================================================================
/// @brief An element of a larger structure which is rectangular.
/// 
/// This class is used extensively to represent pieces of buildings, such as a wall or
/// one plane of a roof.  It provides for keeping track of its orientation and size
/// relative to the larger structure, and for keeping track of texture.

class BuildingRect: public BuildRectData, public VisualElement
{
  friend Gable;
  friend Shed;
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  BuildingRect(void);
  ~BuildingRect(void);
  inline void setSide(int i, float x, float y, float z) 
    {sides[i][0] = x;  sides[i][1] = y;  sides[i][2] = z;} 
  inline void setRelativePos(float x, float y, float z) 
    {relativePos[0] = x;  relativePos[1] = y;  relativePos[2] = z;} 
  void  triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  bool  updateBoundingBox(BoundingBox* box, vec3 offset);
  bool  bufferGeometryOfElement(TriangleBuffer* T, vec3 offset);
  bool  matchRayToElement(vec3& lPos, vec3& lDir, float& lambda, vec3 offset);

private:
  // Instance variables - private
  
  // Member functions - private
  BuildingRect(const BuildingRect&);                 // Prevent copy-construction
  BuildingRect& operator=(const BuildingRect&);      // Prevent assignment

};

#endif




