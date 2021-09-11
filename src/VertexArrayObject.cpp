// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex array object.

#include "VertexArrayObject.h"
#include "Shader.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// =======================================================================================
/// @brief Constructor Create and binds an array of new vertex array objects.
/// @param sizeIn The size of the array of VAOs to create

VertexArrayObject::VertexArrayObject(unsigned sizeIn):size(sizeIn)
{
  vaoArray = new unsigned[size];
  glGenVertexArrays(size, vaoArray);
  glBindVertexArray(vaoArray[0]);
  if(checkGLError(stderr, "VertexArrayObject::VertexArrayObject"))
    exit(-1);

}


// =======================================================================================
/// @brief Destructor
/// Deletes the VAOs as well as the array of their identifiers

VertexArrayObject::~VertexArrayObject(void)
{
  glDeleteVertexArrays(size, vaoArray);
  delete vaoArray;
  if(checkGLError(stderr, "VertexArrayObject::delete"))
    exit(-1);
}


// =======================================================================================
/// @brief Bind the index'th one of our vertex array objects
/// @param index The particular VAO to bind within our array.

void VertexArrayObject::bind(unsigned index)
{
  glBindVertexArray(vaoArray[index]);
  if(checkGLError(stderr, "VertexArrayObject::bind"))
    exit(-1);
}


// =======================================================================================

