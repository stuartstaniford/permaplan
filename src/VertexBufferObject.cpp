// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex buffer object.

#include "VertexBufferObject.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>
#include "Shader.h"


// =======================================================================================
/// @brief Creates and binds a new vertex buffer object of type GL_ARRAY_BUFFER.

/// @brief Note this needs to correspond to the layout in Vertex.h
/// @param count An unsigned number of vertices in the buffer
/// @param data A pointer to count structures of type Vertex to hold the actual vertices
/// @param usage A GLenum to be passed to glBufferData specifying the usage of the data
/// see (man 3 glBufferData)

VertexBufferObject::VertexBufferObject(unsigned count, Vertex* data, GLenum usage)
{
  glGenBuffers(1, &VBOindex);
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  glBufferData(GL_ARRAY_BUFFER, count*sizeof(Vertex), data, usage);
 
  Vertex::vertexLayoutforOpenGL();
  
  if(checkGLError(stderr, "VertexBufferObject::VertexBufferObject"))
    exit(-1);
}


// =======================================================================================
/// @brief Destructor

VertexBufferObject::~VertexBufferObject(void)
{
  glDeleteBuffers(1, &VBOindex);
  if(checkGLError(stderr, "VertexBufferObject::delete"))
    exit(-1);
}


// =======================================================================================
/// @brief Bind our buffer.

void VertexBufferObject::bind(void)
{
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  if(checkGLError(stderr, "VertexBufferObject::bind"))
    exit(-1);
}


// =======================================================================================

