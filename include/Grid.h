// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class draws a square grid at a designated scale over the land surface


#ifndef GRID_H
#define GRID_H

#include <cglm/cglm.h>
#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "LandSurface.h"
#include "LineStripList.h"

// =======================================================================================
// Class variable initialization


class Grid
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Grid(LandSurface& L, float gridSpacing, float alt = 0.0f);
  void newHeight(float z);
  ~Grid(void);
  void draw(void);
  
private:
  
  // Instance variables - private
  float                 spacing;
  LandSurface&          land;
  int                   nX;
  int                   nY;
  unsigned              NHeights;
  float                 sumHeights;
  LineStripList         lines;
  
  // Member functions - private
  void resetAltitude(float alt);

  
};

#endif




