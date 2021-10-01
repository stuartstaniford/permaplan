// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// This class is a very simple one that just creates a square from two
// triangles and textures them from a file.  Draws on the x-y plane,
// with specified width, height.  Useful for in-scene labels and the like,
// after applying an appropriate model transformation.

#include "TexturedRect.h"
#include "VertexBufferObject.h"
#include "Shader.h"
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
/// @brief Helper function to set up a rectangle as two triangles in a vertex buffer.  
/// Needs to be space for six vertices.  All done at z=0.

void bufferRectangle(Vertex* buf, float x, float y,
                  float width, float height, float s, float t, float texW, float texH)
{
  // First triangle
  buf[0].setPosition(x, y, 0.0f);         //lower left
  buf[0].setTexCoords(s, t);
  buf[1].setPosition(x+width, y, 0.0f);   //lower right
  buf[1].setTexCoords(s+texW, t);
  buf[2].setPosition(x, y+height, 0.0f);  //upper left
  buf[1].setTexCoords(s, t+texH);
  
  // Second triangle
  buf[3].copy(buf+2);//upper left
  buf[4].copy(buf+1);//lower right
  buf[5].setPosition(x+width, y+height, 0.0f);  //upper right
  buf[5].setTexCoords(s+texW, t+texH);
}


// =======================================================================================
