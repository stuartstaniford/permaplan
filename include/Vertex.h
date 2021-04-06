// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is where we define our vertex object format.


#ifndef VERTEX_H
#define VERTEX_H

#include <cstring>
#include <cglm/cglm.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Global.h"
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

enum VertexDrawType
{
  FixedColor,
  NoTexColor,
  Lighted
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
// Note: see here for discussion of half precision floating point, which is key
// to keeping our vertex type at or below 32 bytes (1/4 of a cache line on many
// architectures).
// https://clang.llvm.org/docs/LanguageExtensions.html#half-precision-floating-point

class Vertex
{
 public:
  
  // public instance variables
  vec3      pos;
  unsigned  color;
  vec2      tex;
  __fp16    normal[3];
  
  // public methods
  inline void setPosition(float x, float y, float z)
  {
   pos[0] = x;
   pos[1] = y;
   pos[2] = z;
  }

  inline void setPosition(vec3 v)
  {
   glm_vec3_copy(v, pos);
  }
                           
  inline void setTexCoords(float s, float t)
   {
    tex[0] = s;
    tex[1] = t;
   }

  inline void setColor(unsigned u)
   {
    color = u;
   }
  
  inline void setNormal(float x, float y, float z)
  {
   normal[0] = x;
   normal[1] = y;
   normal[2] = z;
  }

  inline void setNormal(vec3 n)
  {
   normal[0] = n[0];
   normal[1] = n[1];
   normal[2] = n[2];
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




