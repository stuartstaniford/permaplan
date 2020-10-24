// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// This is where all concerns to do with the scene lighting live.


#ifndef LIGHTING_MODEL_H
#define LIGHTING_MODEL_H

#include <cglm/cglm.h>
#include "Shader.h"

// =======================================================================================
// Class variable initialization


class LightingModel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  LightingModel(Shader& S);
  void updateGPU(void);
  ~LightingModel(void);
  
private:
  
  // Instance variables - private
  Shader& shader;
  vec3    sunPosition;
  vec3    sunColor;
  float   ambientStrength;

  // Member functions - private
};

#endif




