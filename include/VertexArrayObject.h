// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -

#ifndef VERTEX_ARRAY_OBJECT_H
#define VERTEX_ARRAY_OBJECT_H


// =======================================================================================
/// @brief C++ wrapper around the openGL vertex array object (actually an array of them).

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
  void recreateInNewContext(void);
  void bind(unsigned index);

private:

  // Member functions - private
  VertexArrayObject(const VertexArrayObject&);                 // Prevent copy-construction
  VertexArrayObject& operator=(const VertexArrayObject&);      // Prevent assignment
};

#endif

// =======================================================================================



