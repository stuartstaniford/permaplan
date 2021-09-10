// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This combines a vertex array object and a vertex buffer object, handy
// for quickly instantiating drawable things.

#include "VertexBufferCombo.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>


// =======================================================================================
// Function to create and open a new vertex buffer object

VertexBufferCombo::VertexBufferCombo(unsigned count, Vertex* data, GLenum usage):
                                          VertexArrayObject(1),
                                          VertexBufferObject(count, data, usage)
{
  
}


// =======================================================================================
// Destructor

VertexBufferCombo::~VertexBufferCombo(void)
{
}


// =======================================================================================
// Bind the index'th one of our vertex array objects

void VertexBufferCombo::bind(void)
{
  VertexArrayObject::bind(0);
}



