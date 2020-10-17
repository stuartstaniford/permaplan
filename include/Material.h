// Copyright Staniford Systems.  All Rights Reserved.  October 2020 -
// This class holds the necessary information for a particular kind of material -
// carbon data, texture image for showing the surface of it, etc


#ifndef MATERIAL_H
#define MATERIAL_H

#include <cglm/cglm.h>

// =======================================================================================
// Class variable initialization


class Material
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Material(float carbonD);
  ~Material(void);
  
private:
  
  // Instance variables - private
  float carbonDensity;
  
  // Member functions - private
  
};

#endif




