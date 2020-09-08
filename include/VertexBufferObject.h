// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex array object.


#ifndef VERTEX_BUFFER_OBJECT_H
#define VERTEX_BUFFER_OBJECT_H

#include <cstring>
#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define unless(X) if(!(X))

// =======================================================================================
// Class variable initialization

// =======================================================================================
// Used as the elements within a buffer that will be passed to the shader

class VertexBufElement
{
 public:
  
  // public instance variables
  vec3  pos;
  vec2  tex;
  float accent;
  
  // public methods
  inline void set(float x, float y, float z, float s, float t)
   {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    tex[0] = s;
    tex[1] = t;
    accent = 0.0f;
   }

  inline void set(float x, float y, float z)
  {
   pos[0] = x;
   pos[1] = y;
   pos[2] = z;
   tex[0] = 0.0f;
   tex[1] = 0.0f;
   accent = 0.0f;
  }
  
  inline void setNoTexColor(vec3 C)
   {
    tex[0]  = C[0];
    tex[1]  = C[1];
    accent  = C[2];
   }
  
  inline void fprint(FILE* file)
   {
    fprintf(file, "pos:\t%.1f,\t%.1f,\t%.1f\ttex:\t%.3f\t%.3f\n",
            pos[0], pos[1], pos[2], tex[0], tex[1]);
   }
  
  inline void copy(VertexBufElement* src)
   {
    memcpy(this, src, sizeof(VertexBufElement));
   }
};


// =======================================================================================
// C++ wrapper around the openGL Vertex Buffer Object

class VertexBufferObject
{
 public:
  
  // Instance variables - public

  // Member functions - public
  VertexBufferObject(unsigned count, VertexBufElement* data, GLenum usage);
  ~VertexBufferObject(void);
  void bind();

 private:
  
  // Instance variables - private
  unsigned  VBOindex;
  
  // Member functions - private
  

};

#endif




