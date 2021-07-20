// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef BUILDING_RECT_H
#define BUILDING_RECT_H

#include "VisualElement.h"


// =======================================================================================
/// @brief An element of a larger structure which is rectangular.
/// 
/// This class is used extensively to represent pieces of buildings, such as a wall or
/// one plane of a roof.  It provides for keeping track of its orientation and size
/// relative to the larger structure, and for keeping track of texture.

class BuildingRect: public VisualElement
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  BuildingRect(void);
  ~BuildingRect(void);
  
  inline void setSide(int i, float x, float y, float z) 
    {sides[i][0] = x;  sides[i][1] = y;  sides[i][2] = z;} 
  inline void setPos(float x, float y, float z) 
    {pos[0] = x;  pos[1] = y;  pos[2] = z;} 
  
private:
  
  // Instance variables - private
  vec3 sides[2];
  vec3 pos;
  bool normForward;  // normal is in the direction of sides[0]xsides[1]
  
  // Member functions - private
  BuildingRect(const BuildingRect&);                 // Prevent copy-construction
  BuildingRect& operator=(const BuildingRect&);      // Prevent assignment

};

#endif




