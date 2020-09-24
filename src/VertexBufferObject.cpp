// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This is a C++ wrapper around the openGL vertex buffer object.

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include "VertexBufferObject.h"
#include "Shader.h"

// =======================================================================================
// Function to create and open a new vertex buffer object

VertexBufferObject::VertexBufferObject(unsigned count, VertexBufElement* data, GLenum usage)
{
  glGenBuffers(1, &VBOindex);
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  glBufferData(GL_ARRAY_BUFFER, count*sizeof(VertexBufElement), data, usage);
 
  // vertex data location established
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBufElement),
                        (void*)offsetof(VertexBufElement, pos));
  glEnableVertexAttribArray(0);
  
  // Texture data location established
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexBufElement),
                        (void*)offsetof(VertexBufElement, tex));
  glEnableVertexAttribArray(1);

  // Accent data location established
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(VertexBufElement),
                        (void*)offsetof(VertexBufElement, accent));
  glEnableVertexAttribArray(2);

  if(checkGLError(stderr, "VertexBufferObject::VertexBufferObject"))
    exit(-1);
}


// =======================================================================================
// Destructor

VertexBufferObject::~VertexBufferObject(void)
{
  glDeleteBuffers(1, &VBOindex);
  if(checkGLError(stderr, "VertexBufferObject::delete"))
    exit(-1);
}


// =======================================================================================
// Bind the index'th one of our vertex array objects

void VertexBufferObject::bind(void)
{
  glBindBuffer(GL_ARRAY_BUFFER, VBOindex);
  if(checkGLError(stderr, "VertexBufferObject::bind"))
    exit(-1);
}

// =======================================================================================
// Function for checking whether a ray intersects a triangle.  Uses the Moller-Trumbore
// algorithm.  If true, returns the distance of the intersection along rayDir from
// rayPos as outT.   For more detail, see
// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection

bool mollerTrumbore(vec3 v0, vec3 v1, vec3 v2, vec3 rayPos, vec3 rayDir, float& outT)
{
  vec3 edge1, edge2, h, s, q;
  float a, f, u, v;
  glm_vec3_sub(v1, v0, edge1);
  glm_vec3_sub(v2, v0, edge2);
  glm_vec3_cross(rayDir, edge2, h);
  a = glm_vec3_dot(edge1, h);
  if (a > -epsilon && a < epsilon)
    return false;    // This ray is parallel to this triangle.
  f = 1.0f/a;
  glm_vec3_sub(rayPos, v0, s);
  u = f*glm_vec3_dot(s,h);
  if (u < 0.0f || u > 1.0f)
    return false;
  glm_vec3_cross(s, edge1, q);
  v = f*glm_vec3_dot(rayDir, q);
  if (v < 0.0 || u + v > 1.0)
    return false;
  // At this stage we can compute t to find out where the intersection point is on the line.
  outT = f * glm_vec3_dot(edge2,q);
  if (outT > epsilon) // ray intersection
    return true;
  else // This means that there is a line intersection but not a ray intersection.
    return false;
}


// =======================================================================================

