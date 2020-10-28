// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This combines a vertex array object and a vertex buffer object, handy
// for quickly instantiating drawable things.


#ifndef VERTEX_BUFFER_COMBO_H
#define VERTEX_BUFFER_COMBO_H

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"

// =======================================================================================
// Class variable initialization


class VertexBufferCombo: public VertexArrayObject, public VertexBufferObject
{
  // Instance variables - private
  
  // Member functions - private
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  VertexBufferCombo(unsigned count, Vertex* data, GLenum usage = GL_STATIC_DRAW);
  ~VertexBufferCombo(void);
  void bind(void);
};

#endif




