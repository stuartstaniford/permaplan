// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -

#ifndef LIGHTING_MODEL_H
#define LIGHTING_MODEL_H

#include <cglm/cglm.h>
#include "Shader.h"


// =======================================================================================
/// @brief All concerns to do with the scene lighting live in here.

class LightingModel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  LightingModel(void);
  void updateGPU(void);
  ~LightingModel(void);
  
private:
  
  // Instance variables - private
  vec3    sunPosition;
  vec3    sunColor;
  float   ambientStrength;

  // Member functions - private
  LightingModel(const LightingModel&);                 // Prevent copy-construction
  LightingModel& operator=(const LightingModel&);      // Prevent assignment
};


// =======================================================================================

#endif




