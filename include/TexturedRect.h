// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -

#ifndef TEXTUREDRECT_H
#define TEXTUREDRECT_H

#include "VertexArrayObject.h"
#include "Texture.h"
#include <cglm/cglm.h>


// =======================================================================================
// Forward declarations

class VertexBufferObject;
class Vertex;

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
// Helper C functions

 void bufferRectangle(Vertex* buf, float x, float y,
                  float width, float height, float s, float t, float texW, float texH);

// =======================================================================================

#endif



