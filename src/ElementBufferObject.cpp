// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL element buffer object.

#include "ElementBufferObject.h"
#include "Shader.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// =======================================================================================
/// @brief Constructor creates and binds a new element buffer object
/// @param indices A pointer to an unsigned buffer for the indices of the vertices
/// @param size The number of indices in the buffer
/// @param usage The expected usage of the buffer (see 'man 3 glBufferData' for allowable
/// values of this.

ElementBufferObject::ElementBufferObject(unsigned* indices, unsigned size, GLenum usage)
{
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);

  if(checkGLError(stderr, "ElementBufferObject::ElementBufferObject"))
    exit(-1);
}


// =======================================================================================
/// @brief Destructor

ElementBufferObject::~ElementBufferObject(void)
{
  glDeleteBuffers(1, &ebo);
  if(checkGLError(stderr, "ElementBufferObject::~ElementBufferObject"))
    exit(-1);
}


// =======================================================================================
/// @brief Bind our EBO.

void ElementBufferObject::bind(void)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  if(checkGLError(stderr, "ElementBufferObject::bind"))
    exit(-1);
}


// =======================================================================================
