// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex buffer object.

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include "VertexBufferObject.h"
#include "Shader.h"

// =======================================================================================
// Function to create and open a new vertex buffer object

VertexBufferObject::VertexBufferObject(unsigned count, VertexBufElement* data, GLenum usage)
{
  glGenBuffers(1, &VBOindex);
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  glBufferData(GL_ARRAY_BUFFER, count*sizeof(VertexBufElement), data, usage);
 
  // vertex data location established
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBufElement),
                        (void*)offsetof(VertexBufElement, pos));
  glEnableVertexAttribArray(0);
  
  // Texture data location established
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexBufElement),
                        (void*)offsetof(VertexBufElement, tex));
  glEnableVertexAttribArray(1);

  // Accent data location established
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(VertexBufElement),
                        (void*)offsetof(VertexBufElement, accent));
  glEnableVertexAttribArray(2);

  if(checkGLError(stderr, "VertexBufferObject::VertexBufferObject"))
    exit(-1);
}


// =======================================================================================
// Destructor

VertexBufferObject::~VertexBufferObject(void)
{
  glDeleteBuffers(1, &VBOindex);
  if(checkGLError(stderr, "VertexBufferObject::delete"))
    exit(-1);
}


// =======================================================================================
// Bind the index'th one of our vertex array objects

void VertexBufferObject::bind(void)
{
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  if(checkGLError(stderr, "VertexBufferObject::bind"))
    exit(-1);
}


