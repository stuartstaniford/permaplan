// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex array object.

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "ElementBufferObject.h"
#include "Shader.h"

// =======================================================================================
// Function to create and open a new vertex array object

ElementBufferObject::ElementBufferObject(unsigned* indices, unsigned size, GLenum usage)
{
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);

  if(checkGLError(stderr, "ElementBufferObject::ElementBufferObject"))
    exit(-1);

}


// =======================================================================================
// Destructor

ElementBufferObject::~ElementBufferObject(void)
{
  glDeleteBuffers(1, &ebo);
  if(checkGLError(stderr, "ElementBufferObject::~ElementBufferObject"))
    exit(-1);
}


// =======================================================================================
// Bind the index'th one of our vertex array objects

void ElementBufferObject::bind(void)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  if(checkGLError(stderr, "ElementBufferObject::bind"))
    exit(-1);
}


