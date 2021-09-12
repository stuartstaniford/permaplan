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
/// @brief Recreate our necessary state in a new OpenGL context (eg a new window).
///
/// When we create a new OpenGL context and share the objects from the old one (eg a new
/// window with a different viewpoint on the same scene), OpenGL only shares basic
/// objects and not container objects.  The purpose of this function is to recreate the
/// container objects so that we can draw the contents of this TriangleBuffer in the new
/// context.
/// See this link for more details:
///https://stackoverflow.com/questions/55885139/what-is-shareable-between-opengl-contexts-and-how-to-enable-sharing

void VertexArrayObject::recreateInNewContext(void)
{
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

