// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class just draws X, Y, and Z axes in red, green, and blue fixed colors

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ColoredAxes.h"


// =======================================================================================
// Create the three axes to display in color later

ColoredAxes::ColoredAxes(float axesLen):
                                        LineStripList()
{
  unsigned theColor;
  unsigned char* bytes = (unsigned char*)(&theColor);
  vec3 pos      = {0.0f, 0.0f, 0.0f};
  vec3 dir      = {0.0f, 0.0f, 0.0f};
  
  for(int i = 0; i < 3; i++)
   {
    theColor = 0u;
    bytes[i] = 0xff;
    dir[i]      = axesLen;
    addLine(pos, dir, theColor);
    dir[i]      = 0.0f;
   }
  sendToGPU();
  if(checkGLError(stderr, "ColoredAxes::ColoredAxes"))
    exit(-1);
}


// =======================================================================================
// Destructor

ColoredAxes::~ColoredAxes(void)
{
}


// =======================================================================================
