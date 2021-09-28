// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class just draws X, Y, and Z axes in red, green, and blue fixed colors

#include "ColoredAxes.h"
#include "Shader.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>


// =======================================================================================
/// @brief Constructor. Create the three axes to display in color later (via 
/// LineStripList::draw).
/// @param axesLen The length of the axes to draw (in spaceUnits).

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
/// @brief Destructor

ColoredAxes::~ColoredAxes(void)
{
}


// =======================================================================================
