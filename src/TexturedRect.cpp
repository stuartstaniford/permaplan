// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// This class is a very simple one that just creates a square from two
// triangles and textures them from a file.  Draws on the x-y plane,
// with specified width, height.  Useful for in-scene labels and the like,
// after applying an appropriate model transformation.

#include "TexturedRect.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// =======================================================================================
/// @brief Constructor sets up two triangles with co-ords and texture coordss.
/// @param textureFileName A C-string with the name of the file to find the texture image.
/// @param widthIn The width to make the rectangle.
/// @param heightIn The height to make the rectangle

TexturedRect::TexturedRect(const char* textureFileName, float widthIn,
                           float heightIn):
                                width(widthIn),
                                height(heightIn),
                                texture(textureFileName, 1),
                                axesVAOs(1)
{
  Vertex buf[6];

  texture.sendToGpu();
  
  if(height == 0.0f)  //infer the height from the width and the aspect ratio of the texture
    height = (width*texture.height)/texture.width;
    
  axesVAOs.bind(0);

  bufferRectangle(buf, 0.0f, 0.0f, width, height, 0.0f, 0.0f, 1.0f, 1.0f);
  
  VBO = new VertexBufferObject(6, buf, GL_STATIC_DRAW);

  if(checkGLError(stderr, "TexturedRect::TexturedRect"))
    exit(-1);
}


// =======================================================================================
/// @brief Destructor

TexturedRect::~TexturedRect(void)
{
  delete VBO;
}



// =======================================================================================
/// @brief Render our rectangle

void TexturedRect::draw(void)
{
  axesVAOs.bind(0);
  texture.bind(0, "earthTexture");
  glDrawArrays(GL_TRIANGLES, 0, 6);

  if(checkGLError(stderr, "TexturedRect::draw"))
    exit(-1);
}


// =======================================================================================
