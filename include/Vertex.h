// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is where we define our vertex object format.


#ifndef VERTEX_H
#define VERTEX_H

#include <cstring>
#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GlobalMacros.h"
#include "Logging.h"


// =======================================================================================
// An Enum for how much of the Vertex to fill out when returning from things like
// getNextVertex.

enum VertexDetail
{
  PositionOnly,
  IncludeTexture,
  IncludeNormal
};


// =======================================================================================
// Helper function prototypes

bool mollerTrumbore(vec3 v0, vec3 v1, vec3 v2, vec3 rayPos, vec3 rayDir, float& outT);

inline bool mollerTrumbore(vec3 triangle[3], vec3 rayPos, vec3 rayDir, float& outT)
{
  return mollerTrumbore(triangle[0], triangle[1], triangle[2], rayPos, rayDir, outT);
}

// =======================================================================================
// Used as the elements within a buffer that will be passed to the shader

class Vertex
{
 public:
  
  // public instance variables
  vec3    pos;
  vec2    tex;
  __fp16  norm[3];
  __fp16  accent;
  
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
  
  inline void copy(Vertex* src)
   {
    memcpy(this, src, sizeof(Vertex));
   }
  
  // Function for checking whether a ray intersects a triangle and the triangle is
  // specified by pointers to two other Vertex objects besides *this.  If true,
  // returns the distance of the intersection along rayDir from rayPos as outT.
  inline bool rayMatch(Vertex* v1, Vertex* v2, vec3 rayPosition,
                vec3 rayDirection, float& outT)
   {
    return mollerTrumbore(this->pos, v1->pos, v2->pos, rayPosition, rayDirection, outT);
   }
  
};

#endif




