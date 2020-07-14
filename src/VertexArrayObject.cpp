// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex array object.

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "VertexArrayObject.h"
#include "Shader.h"

// =======================================================================================
// Function to create and open a new vertex array object

VertexArrayObject::VertexArrayObject(unsigned sizeIn):size(sizeIn)
{
  vaoArray = new unsigned[size];
  glGenVertexArrays(size, vaoArray);
  glBindVertexArray(vaoArray[0]);
  if(checkGLError(stderr, "VertexArrayObject::VertexArrayObject"))
    exit(-1);

}


// =======================================================================================
// Destructor

VertexArrayObject::~VertexArrayObject(void)
{
  glDeleteVertexArrays(size, vaoArray);
  delete vaoArray;
  if(checkGLError(stderr, "VertexArrayObject::delete"))
    exit(-1);
}


// =======================================================================================
// Bind the index'th one of our vertex array objects

void VertexArrayObject::bind(unsigned index)
{
  glBindVertexArray(vaoArray[index]);
  if(checkGLError(stderr, "VertexArrayObject::bind"))
    exit(-1);
}


