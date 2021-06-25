// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This combines a vertex array object and a vertex buffer object, handy
// for quickly instantiating drawable things.

#include "ElementBufferCombo.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


// =======================================================================================
// Function to create and open a new vertex buffer object

ElementBufferCombo::ElementBufferCombo(Vertex* vertices, unsigned vCount,
                          unsigned* indices, unsigned iCount, GLenum usage):
                          VertexArrayObject(1),
                          VertexBufferObject(vCount, vertices, usage),
                          ElementBufferObject(indices, iCount, usage)
{

}


// =======================================================================================
// Destructor

ElementBufferCombo::~ElementBufferCombo(void)
{
}


// =======================================================================================
// Bind the index'th one of our vertex array objects

void ElementBufferCombo::bind(void)
{
  VertexArrayObject::bind(0);
  VertexBufferObject::bind();
  ElementBufferObject::bind();
}



