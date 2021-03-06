// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex buffer object.

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include "VertexBufferObject.h"
#include "Shader.h"

// =======================================================================================
// Function to create and open a new vertex buffer object.
// Note this needs to correspond to the layout in Vertex.h

VertexBufferObject::VertexBufferObject(unsigned count, Vertex* data, GLenum usage)
{
  glGenBuffers(1, &VBOindex);
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  glBufferData(GL_ARRAY_BUFFER, count*sizeof(Vertex), data, usage);
 
  // vertex data location established
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, pos));
  glEnableVertexAttribArray(0);
  
  // Color data location established
  glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                        (void*)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);

  // Texture data location established
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, tex));
  glEnableVertexAttribArray(2);

  // Normal data location established
  glVertexAttribPointer(3, 3, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(3);

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


// =======================================================================================

