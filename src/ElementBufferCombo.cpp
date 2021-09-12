// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// Combines a vertex array object, a vertex buffer object, and an element buffer
// object.  This class provides all the OpenGL objects necessary to draw some vertices 
// via indices into the triangles.

#include "ElementBufferCombo.h"
#include <cstdio>
#include <stdexcept>
#include <err.h>


// =======================================================================================
/// @brief Constructor - initializes and binds all three of our component OpenGL objects.
/// @param vertices - Pointer to an array of Vertex objects
/// @param vCount - the count of vertices in the vertices array
/// @param indices - Pointer to an array of unsigned indices
/// @param iCount - the count of indices we have.
/// @param usage - a GLenum with the anticipated usage of the data (see for example
/// man 3 glBufferData).

ElementBufferCombo::ElementBufferCombo(Vertex* vertices, unsigned vCount,
                          unsigned* indices, unsigned iCount, GLenum usage):
                          VertexArrayObject(1),
                          VertexBufferObject(vCount, vertices, usage),
                          ElementBufferObject(indices, iCount, usage)
{

}


// =======================================================================================
/// @brief Destructor

ElementBufferCombo::~ElementBufferCombo(void)
{
}


// =======================================================================================
/// @brief Bind all the things.

void ElementBufferCombo::bind(void)
{
  VertexArrayObject::bind(0);
  VertexBufferObject::bind();
  ElementBufferObject::bind();
}



