// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex array object.


#ifndef VERTEX_BUFFER_OBJECT_H
#define VERTEX_BUFFER_OBJECT_H

#include <cstring>
#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GlobalMacros.h"


// =======================================================================================
// Class variable initialization


// =======================================================================================
// Helper function prototypes

bool mollerTrumbore(vec3 v0, vec3 v1, vec3 v2, vec3 rayPos, vec3 rayDir, float& outT);


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
  inline void set(float x, float y, float z, float s, float t, float a)
  {
   pos[0] = x;
   pos[1] = y;
   pos[2] = z;
   tex[0] = s;
   tex[1] = t;
   accent = a;
  }

  inline void set(float x, float y, float z, float s, float t)
   {
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    tex[0] = s;
    tex[1] = t;
    accent = 0.0f;
   }

  inline void set(vec3 v, vec3 c)
   {
    pos[0] = v[0];
    pos[1] = v[1];
    pos[2] = v[2];
    tex[0] = c[0];
    tex[1] = c[1];
    accent = c[2];
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
  
  // Function for checking whether a ray intersects a triangle and the triangle is
  // specified by pointers to two other VertexBufElements besides *this.  If true,
  // returns the distance of the intersection along rayDir from rayPos as outT.
  inline bool rayMatch(VertexBufElement* v1, VertexBufElement* v2, vec3 rayPosition,
                vec3 rayDirection, float& outT)
   {
    return mollerTrumbore(this->pos, v1->pos, v2->pos, rayPosition, rayDirection, outT);
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




