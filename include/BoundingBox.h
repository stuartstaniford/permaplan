// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is used for anything that has a axis-aligned 3D bounding box
// around it (eg for use in first cut intersection detection).


#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <cglm/cglm.h>
#include "Shader.h"
#include "VertexBufferCombo.h"
#include "HttpDebug.h"


// =======================================================================================
// Class variable initialization


class BoundingBox
{
 public:
  
  // Instance variables - public
  vec3        lower;
  vec3        upper;

  // Member functions - public
  BoundingBox(vec3& bottomCorner, vec3& topCorner);
  BoundingBox(float x_l, float y_l, float z_l, float x_u, float y_u, float z_u);
  BoundingBox(void); // special constructor for vertex search
  ~BoundingBox(void);
  void hugeValify(void);
  void reset(float x_l, float y_l, float z_l, float x_u, float y_u, float z_u);
  inline void reset(vec3& bC, vec3& tC) {glm_vec3_copy(bC, lower);glm_vec3_copy(tC, upper);}
  void bufferGeometry(VertexBufElement* buf);
  VertexBufferCombo* bufferGeometry(void);
  void draw(Shader& shader, VertexBufferCombo* combo, vec4 color);
  bool matchRay(vec3& position, vec3& direction, float& lambda);
  bool xyContains(const BoundingBox& otherBox);
  bool extendZ(const BoundingBox& otherBox);
  void unsetZs(void);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
};


// =======================================================================================
// Utility functions

inline void setVec3(vec3 vec, float x, float y, float z)
{
  vec[0] = x;
  vec[1] = y;
  vec[2] = z;
}

inline void setVec2(vec2 vec, float x, float y)
{
  vec[0] = x;
  vec[1] = y;
}


#endif




