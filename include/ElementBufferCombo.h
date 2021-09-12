// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -

#ifndef ELEMENT_BUFFER_COMBO_H
#define ELEMENT_BUFFER_COMBO_H

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"


// =======================================================================================
/// @brief Combines a vertex array object, a vertex buffer object, and an element buffer
/// object.
/// 
/// This class provides all the OpenGL objects necessary to draw some vertices via 
/// indices into the triangles

class ElementBufferCombo: public VertexArrayObject, public VertexBufferObject,
        public ElementBufferObject
{
  // Instance variables - private
  
  // Member functions - private
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  ElementBufferCombo(Vertex* vertices, unsigned vCount, unsigned* indices,
                      unsigned iCount, GLenum usage);
  ~ElementBufferCombo(void);
  void bind(void);

private:
  
  // Member functions - private
  ElementBufferCombo(const ElementBufferCombo&);                 // Prevent copy-construction
  ElementBufferCombo& operator=(const ElementBufferCombo&);      // Prevent assignment

};


// =======================================================================================

#endif




