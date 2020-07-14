// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex array object.


#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H

// =======================================================================================
// Class variable initialization


class VertexArrayObject
{
  // Instance variables - private
  unsigned*  vaoArray;
  
  // Member functions - private
  
  public:
  
  // Instance variables - public
  unsigned   size;

  // Member functions - public
  VertexArrayObject(unsigned sizeIn);
  ~VertexArrayObject(void);
  void bind(unsigned index);
};

#endif




