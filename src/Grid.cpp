
// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class draws a square grid at a designated scale over the land surface

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Grid.h"


// Useful constants that define how the grid works and looks.

vec4 gridColor[3] = { {0.6f, 0.5f, 0.0f, 0.5f},  // transparent yellowy orange
                      {0.85f, 0.35f, 0.0f, 0.7f},  // redder and less transparent
                      {0.95f, 0.15f, 0.0f, 0.85f}  // even more so
                     };

float gridHeights[3] = {0.2f, 0.4f, 0.6f};

int gridModuli[2] = {10, 100};

// =======================================================================================
// Create the GPU buffers containing the grid lines to display in color later

#define setGridLevel(i)     if(!((i)%gridModuli[1])) \
                              level = 2; \
                            else if(!((i)%gridModuli[0])) \
                              level = 1; \
                            else \
                               level = 0

Grid::Grid(Shader& S, LandSurface& L, float gridSpacing, float alt):
                      spacing(gridSpacing),
                      land(L),
                      NHeights(0u),
                      sumHeights(0.0f),
                      lines(S)
{
  if(!land.rect)
    err(-1, "Unsupported lack of rect in Grid::Grid\n");
  
  resetAltitude(alt);
  if(checkGLError(stderr, "Grid::Grid"))
    exit(-1);
}


// =======================================================================================
// Redo us at a different height

void Grid::resetAltitude(float alt)
{
  nX = (int)(land.rect->width/spacing) + 1;
  nY = (int)(land.rect->height/spacing) + 1;
  
  int i; float f;
  int level;
  vec3 pos, dir;
  
  // north south lines
  pos[1] = 0.0f;
  dir[0] = 0.0f;
  dir[1] = land.rect->height;
  dir[2] = 0.0f;
  for(i = 0, f = 0.0f; i < nX; i++, f+=spacing)
   {
    setGridLevel(i);
    pos[0] = f;
    pos[2] = alt + gridHeights[level];
    lines.addLine(pos, dir, gridColor[level]);
   }
  
  // east west lines
  pos[0] = 0.0f;
  dir[0] = land.rect->width;
  dir[1] = 0.0f;
  dir[2] = 0.0f;
  for(i = 0, f = 0.0f; i < nY; i++, f+=spacing)
   {
    setGridLevel(i);
    pos[1] = f;
    pos[2] = alt + gridHeights[level];
    lines.addLine(pos, dir, gridColor[level]);
   }
  lines.sendToGPU();
}


// =======================================================================================
// New height (the grid will be at the average of these).

void Grid::newHeight(float z)
{
  NHeights++;
  sumHeights += z;
  resetAltitude(sumHeights/NHeights);
}


// =======================================================================================
// Destructor

Grid::~Grid(void)
{
}



// =======================================================================================
// Render our part of the scene


void Grid::draw(void)
{
  lines.draw();
  if(checkGLError(stderr, "Grid::draw"))
    exit(-1);
}


// =======================================================================================
