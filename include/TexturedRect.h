// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -

#ifndef TEXTUREDRECT_H
#define TEXTUREDRECT_H

#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Texture.h"
#include <cglm/cglm.h>


// =======================================================================================
/// @brief Create/render a textured rectangle.
///
/// This class is a very simple one that just creates a square from two triangles and 
/// textures them from a file.  Draws on the x-y plane, with specified width, height.  
/// @todo This code is very early, doesn't use TriangleBuffers, and has it's own VAO
/// just for one square.  It works fine, but should be refactored or eliminated at some 
/// point for the sake of uniformity and performance.

class TexturedRect
{
 public:
  
  // Instance variables - public
  float     width;
  float     height;
  Texture   texture;

  // Member functions - public
  TexturedRect(const char* textureFileName, float widthIn, float heightIn);
  ~TexturedRect(void);
  void draw(void);

 private:
  
  // Instance variables - private
  VertexArrayObject     axesVAOs;
  VertexBufferObject*   VBO;
  
  // Member functions - private
  TexturedRect(const TexturedRect&);                 // Prevent copy-construction
  TexturedRect& operator=(const TexturedRect&);      // Prevent assignment
};


// =======================================================================================
// Helper function to set up a rectangle as two triangles in a vertex buffer.  Needs
// to be space for six vertices.  All done at z=0.

inline void bufferRectangle(Vertex* buf, float x, float y,
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

#endif



