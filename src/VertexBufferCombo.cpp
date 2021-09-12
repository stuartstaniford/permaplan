// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This combines a vertex array object and a vertex buffer object, handy
// for quickly instantiating drawable things.

#include "VertexBufferCombo.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>


// =======================================================================================
/// @brief Constuctor will create and bind a new vertex buffer combo
/// 
/// @param count An unsigned number of vertices in the buffer
/// @param data A pointer to count structures of type Vertex to hold the actual vertices
/// @param usage A GLenum to be passed to glBufferData specifying the usage of the data
/// see (man 3 glBufferData)

VertexBufferCombo::VertexBufferCombo(unsigned count, Vertex* data, GLenum usage):
                                          VertexArrayObject(1),
                                          VertexBufferObject(count, data, usage)
{
  
}


// =======================================================================================
/// @brief Destructor

VertexBufferCombo::~VertexBufferCombo(void)
{
}


// =======================================================================================
/// @brief Bind our vertex array object

void VertexBufferCombo::bind(void)
{
  VertexArrayObject::bind(0);
}



