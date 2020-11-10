// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This combines a vertex array object and a vertex buffer object, handy
// for quickly instantiating drawable things.


#ifndef ELEMENT_BUFFER_COMBO_H
#define ELEMENT_BUFFER_COMBO_H

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

// =======================================================================================
// Class variable initialization


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

#endif




