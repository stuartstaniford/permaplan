// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -

#ifndef ELEMENT_BUFFER_OBJECT_H
#define ELEMENT_BUFFER_OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>


// =======================================================================================
/// @brief C++ wrapper around the openGL element buffer object.
/// 
/// https://learnopengl.com/Getting-started/Hello-Triangle


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


// =======================================================================================

#endif




