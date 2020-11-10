// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This is a C++ wrapper around the openGL element buffer object.


#ifndef ELEMENT_BUFFER_OBJECT_H
#define ELEMENT_BUFFER_OBJECT_H

// =======================================================================================
// Class variable initialization


class ElementBufferObject
{
 public:
  
  // Instance variables - public
  
  // Member functions - public
  ElementBufferObject(unsigned* indices, unsigned size, GLenum usage);
  ~ElementBufferObject(void);
  void bind(void);

 private:
  
  // Instance variables - private
  unsigned  ebo;
  
  // Member functions - private
  ElementBufferObject(const ElementBufferObject&);                 // Prevent copy-construction
  ElementBufferObject& operator=(const ElementBufferObject&);      // Prevent assignment

};

#endif




