
// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class draws a square grid at a designated scale over the land surface

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Grid.h"


// Useful constants that define how the grid works and looks.
/*
vec4 gridColor[3] = { 0.6f, 0.5f, 0.0f, 0.5f,  // transparent yellowy orange
                      0.75f, 0.45f, 0.0f, 0.7f,  // redder and less transparent
                      0.85f, 0.35f, 0.0f, 0.85f  // even more so
                    };

float gridHeights[3] = {0.2f, 0.4f, 0.6f};

int gridModuli[2] = {10, 100}
*/

vec4 gridColor = { 0.6f, 0.5f, 0.0f, 0.5f};  // transparent yellowy orange
  
// =======================================================================================
// Create the GPU buffers containing the grid lines to display in color later

Grid::Grid(Shader& S, LandSurface& L, float gridSpacing): shader(S), spacing(gridSpacing),
            land(L), axesVAOs(1)
{
  VertexBufElement* buf;
  
  if(!land.rect)
    err(-1, "Unsupported lack of rect in Grid::Grid\n");
  
  nX = (int)(land.rect->width/spacing) + 1;
  nY = (int)(land.rect->height/spacing) + 1;

  buf = new VertexBufElement[2*(nX+nY)];
  if(!buf)
    err(-1, "Couldn't allocate memory in Grid::Grid\n");
  axesVAOs.bind(0);

  int i; float f;
  for(i = 0, f = 0.0f; i < nX; i++, f+=spacing)
   {
    buf[2*i].set(f, 0.0f, 0.5f, 0.0f, 0.0f);
    buf[2*i+1].set(f, land.rect->height, 0.5f, 0.0f, 0.0f);
   }

  for(i = nX, f = 0.0f; i < nX+nY; i++, f+=spacing)
   {
    buf[2*i].set(0.0f, f, 0.5f, 0.0f, 0.0f);
    buf[2*i+1].set(land.rect->width, f, 0.5f, 0.0f, 0.0f);
   }

  VBO = new VertexBufferObject(2*(nX+nY), buf, GL_STATIC_DRAW);
  delete buf;

  if(checkGLError(stderr, "Grid::Grid"))
    exit(-1);
}


// =======================================================================================
// Destructor

Grid::~Grid(void)
{
  delete VBO;
}



// =======================================================================================
// Render our part of the scene


void Grid::draw(void)
{
  shader.setUniform("fixedColor", true);
  shader.setUniform("theColor", gridColor);
  axesVAOs.bind(0);
  glDrawArrays(GL_LINES, 0, 2*(nX+nY));
  shader.setUniform("fixedColor", false);
  if(checkGLError(stderr, "Grid::draw"))
    exit(-1);
}


// =======================================================================================
