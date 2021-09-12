// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -

#ifndef VERTEX_BUFFER_COMBO_H
#define VERTEX_BUFFER_COMBO_H

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"


// =======================================================================================
/// @brief Combines a vertex array object and a vertex buffer object.
/// 
/// This class is handy for quickly instantiating drawable things.

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

private:

  // Member functions - private
  VertexBufferCombo(const VertexBufferCombo&);                 // Prevent copy-construction
  VertexBufferCombo& operator=(const VertexBufferCombo&);      // Prevent assignment
};


// =======================================================================================

#endif




