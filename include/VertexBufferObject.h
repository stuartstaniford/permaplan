// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex array object.


#ifndef VERTEX_BUFFER_OBJECT_H
#define VERTEX_BUFFER_OBJECT_H

#include <cstring>
#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GlobalMacros.h"
#include "Logging.h"
#include "Vertex.h"


// =======================================================================================
// Class variable initialization


// =======================================================================================
// C++ wrapper around the openGL Vertex Buffer Object

class VertexBufferObject
{
 public:
  
  // Instance variables - public

  // Member functions - public
  VertexBufferObject(unsigned count, Vertex* data, GLenum usage);
  ~VertexBufferObject(void);
  void bind();

 private:
  
  // Instance variables - private
  unsigned  VBOindex;
  
  // Member functions - private
  

};

#endif




