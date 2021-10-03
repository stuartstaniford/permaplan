// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is where we define our vertex object format.


#ifndef VERTEX_H
#define VERTEX_H

#include "Global.h"
#include "Logging.h"
#include <cstring>
#include <stdio.h>
#include <cglm/cglm.h>


// =======================================================================================
/// @brief An Enum for how much of the Vertex to fill out when returning from things like
/// getNextVertex.

enum VertexDetail
{
  PositionOnly,   ///< Only provide position vectors
  IncludeTexture, ///< In addition, include texture co-ords (but not normals)
  IncludeNormal   ///< Provide everything including normals
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
/// @brief Class for storing information about a single vertex.
/// 
///  Used as the elements within a buffer that will be passed to the shader
/// Note: see [this discussion]
/// (https://clang.llvm.org/docs/LanguageExtensions.html#half-precision-floating-point) 
/// of half precision floating point, which is key to keeping our vertex type at or 
/// below 32 bytes (1/4 of a cache line on many architectures).

class Vertex
{
 public:
  
  // public instance variables
  vec3      pos;
  unsigned  color;
  vec2      tex;
  __fp16    normal[3];
#ifdef LOG_VALID_TRIANGLE_BUFS
  unsigned short objectId;
#endif
  
  // public methods
  
  /// @brief set the position of this Vertex 
  /// @param x - the X value of pos
  /// @param y - the Y value of pos
  /// @param z - the Z value of pos
  inline void setPosition(float x, float y, float z)
  {
   pos[0] = x;
   pos[1] = y;
   pos[2] = z;
  }

  /// @brief set the position of this Vertex 
  /// @param v A vec3 which will be copied into the position field
  inline void setPosition(vec3 v)
  {
   glm_vec3_copy(v, pos);
  }
                           
  /// @brief set the texture coordinates of this Vertex
  /// @param s - the horizontal texture coordinate
  /// @param t - the vertical texture coordinate
  inline void setTexCoords(float s, float t)
   {
    tex[0] = s;
    tex[1] = t;
   }

  /// @brief set the color of this Vertex
  /// @param u - an unsigned interpreted as a four byte RGBA array.
  inline void setColor(unsigned u)
   {
    color = u;
   }
  
  /// @brief set the color of this Vertex
  /// @param u - an unsigned interpreted as a four byte RGBA array.
  inline void setNormal(float x, float y, float z)
  {
   normal[0] = x;
   normal[1] = y;
   normal[2] = z;
  }

  /// @brief set the normal vector of this Vertex
  /// @param n - a vec3 with the normal direction.  Note, although supplied as a
  /// float vector, it will be converted to half precision interally in the vertex 
  /// structure.  Note also that normals will be normalized in the fragment shader,
  /// so it is not necessary that they be a unit vector here.
  inline void setNormal(vec3 n)
  {
   normal[0] = n[0];
   normal[1] = n[1];
   normal[2] = n[2];
  }

  /// @brief Store the low 16 bits of an OID in the Vertex.
  /// 
  /// This can be used to detect errors in the TriangleBuffer instances, such as
  /// triangles split across different objects.
  /// @param oid - an unsigned objectId
  inline void setObjectId(unsigned oid)
   {
#ifdef LOG_VALID_TRIANGLE_BUFS
    objectId = 0x0000ffff&oid;
#endif
   }
  
  /// @brief Print the vertex on one line of a file with tab separated fields
  /// @param file The FILE* pointer to an open file.
  inline void fprint(FILE* file)
   {
    fprintf(file, "pos:\t%.1f,\t%.1f,\t%.1f\ttex:\t%.3f\t%.3f\n",
            pos[0], pos[1], pos[2], tex[0], tex[1]);
   }
  
  /// @brief Copy another Vertex to this one.
  /// @param src A pointer to the other Vertex to be copied.
  inline void copy(Vertex* src)
   {
    memcpy(this, src, sizeof(Vertex));
   }
  
  /// @brief Check whether a ray intersects a triangle.
  /// 
  /// The triangle is specified by pointers to two other Vertex objects besides *this.  
  /// @returns True if the ray does intersect the triangle, false otherwise.
  /// @param v1 First Vertex of the triangle (besides *this)
  /// @param v2 Second Vertex
  /// @param rayPosition A position somewhere along the ray to be matched
  /// @param rayDirection A vector in the direction of the ray to be matched
  /// @param, outT Reference to a float which will be used to return the multiple of 
  /// the intersection along rayDir from rayPos.
  inline bool rayMatch(Vertex* v1, Vertex* v2, vec3 rayPosition,
                                                    vec3 rayDirection, float& outT)
   {
    return mollerTrumbore(this->pos, v1->pos, v2->pos, rayPosition, rayDirection, outT);
   }
  
  static void vertexLayoutforOpenGL(void);
  static void printVertexTableHeader(FILE* file);
  void printVertexTableRow(FILE* file, unsigned row);
};

#endif




