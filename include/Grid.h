// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class draws a square grid at a designated scale over the land surface


#ifndef GRID_H
#define GRID_H

#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "LandSurface.h"
#include <cglm/cglm.h>

// =======================================================================================
// Class variable initialization


class Grid
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Grid(Shader& S, LandSurface& L, float gridSpacing);
  ~Grid(void);
  void draw(void);
  
private:
  
  // Instance variables - private
  Shader&               shader;
  float                 spacing;
  LandSurface&          land;
  VertexArrayObject     axesVAOs;
  VertexBufferObject*   VBO;
  int                   nX;
  int                   nY;
  
  // Member functions - private
  
  
};

#endif




