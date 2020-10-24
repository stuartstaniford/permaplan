
// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// This is where all concerns to do with the scene lighting live.

#include "LightingModel.h"


// Useful constants.

#define SUN_DISTANCE 100000.0f

// =======================================================================================
// Constructor

LightingModel::LightingModel(Shader& S):
                    shader(S),
                    ambientStrength(0.2f)
{
  sunPosition[0]  = -SUN_DISTANCE/M_SQRT2;
  sunPosition[1]  = 0.0f;
  sunPosition[2]  = SUN_DISTANCE/M_SQRT2;
  sunColor[0]     = 1.0f;
  sunColor[0]     = 1.0f;
  sunColor[1]     = 1.0f;
}


// =======================================================================================
// Update the light sources on the GPU.

void LightingModel::updateGPU(void)
{
  shader.setUniform("sunPosition",      sunPosition);
  shader.setUniform("sunColor",         sunColor);
  shader.setUniform("ambientStrength",  ambientStrength);
  if(checkGLError(stderr, "LightingModel::updateGPU"))
    exit(-1);
}


// =======================================================================================
// Destructor

LightingModel::~LightingModel(void)
{
}


// =======================================================================================
