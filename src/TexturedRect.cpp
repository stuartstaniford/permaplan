// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// This class is a very simple one that just creates a square from two
// triangles and textures them from a file.  Draws on the x-y plane,
// with specified width, height.  Useful for in-scene labels and the like,
// after applying an appropriate model transformation.

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "TexturedRect.h"


// =======================================================================================
// Set up two triangles with co-ords and texture coords

TexturedRect::TexturedRect(Shader& S, const char* textureFileName, float widthIn,
                           float heightIn):
                                shader(S),
                                width(widthIn),
                                height(heightIn),
                                texture(textureFileName),
                                axesVAOs(1)
{
  Vertex buf[6];

  if(height == 0.0f)  //infer the height from the width and the aspect ratio of the texture
    height = (width*texture.height)/texture.width;
    
  axesVAOs.bind(0);

  bufferRectangle(buf, 0.0f, 0.0f, width, height, 0.0f, 0.0f, 1.0f, 1.0f);
  
  VBO = new VertexBufferObject(6, buf, GL_STATIC_DRAW);

  if(checkGLError(stderr, "TexturedRect::TexturedRect"))
    exit(-1);
}


// =======================================================================================
// Destructor

TexturedRect::~TexturedRect(void)
{
  delete VBO;
}



// =======================================================================================
// Render our part of the scene

void TexturedRect::draw(void)
{
  axesVAOs.bind(0);
  texture.bind(shader, 0, "earthTexture");
  glDrawArrays(GL_TRIANGLES, 0, 6);

  if(checkGLError(stderr, "TexturedRect::draw"))
    exit(-1);
}


// =======================================================================================
