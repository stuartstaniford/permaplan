// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class just draws X, Y, and Z axes in red, green, and blue fixed colors

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ColoredAxes.h"


// =======================================================================================
// Create three small GPU buffers containing the three axes to display in color later

ColoredAxes::ColoredAxes(Shader& S, float axesLen):
                            shader(S),
                            axesVAOs(3),
                            axesLength(axesLen)
{
  VertexBufElement buf[2];
  
  for(int i = 0; i < 3; i++)
   {
    axesVAOs.bind(i);
    buf[0].set(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    buf[1].set(0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    buf[1].pos[i] = axesLength;
    VBO[i] = new VertexBufferObject(2, buf, GL_STATIC_DRAW);
   }
  if(checkGLError(stderr, "ColoredAxes::ColoredAxes"))
    exit(-1);
}


// =======================================================================================
// Destructor

ColoredAxes::~ColoredAxes(void)
{
  for(int i = 0; i < 3; i++)
    delete VBO[i];
}



// =======================================================================================
// Render our part of the scene

vec4  blackColor  = {0.0f, 0.0f, 0.0f, 1.0f};

void ColoredAxes::draw(void)
{
  vec4 theColor;
  shader.setUniform("fixedColor", true);

  //fprintf(stderr, "Drawing colored axes\n");
  
  for(int i = 0; i < 3; i++)
   {
    glm_vec4_copy(blackColor, theColor);
    theColor[i] = 1.0f;
    shader.setUniform("theColor", theColor);
    axesVAOs.bind(i);
    glDrawArrays(GL_LINE_STRIP, 0, 2);
   }
  shader.setUniform("fixedColor", false);
  if(checkGLError(stderr, "ColoredAxes::draw"))
    exit(-1);
}


// =======================================================================================
