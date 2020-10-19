// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Class for displaying a set of colored lines, used for visual indicators (axes, grids,
// rays, etc).


#ifndef LINE_STRIP_LIST_H
#define LINE_STRIP_LIST_H

#include <vector>
#include <cglm/cglm.h>
#include "VertexBufferCombo.h"
#include "Shader.h"
#include "HttpDebug.h"

// =======================================================================================
// Class variable initialization


class LineStripList: public std::vector<Vertex>
{
 public:
  
  // Instance variables - public

  // Member functions - public
  LineStripList(Shader& S, GLenum usage = GL_STATIC_DRAW);
  ~LineStripList();
  void addLine(vec3 pos, vec3 dir, vec4 color);
  void sendToGPU(void);
  void draw(void);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  Shader&             shader;
  VertexBufferCombo*  combo;
  GLenum              usageGL;
  unsigned            N;       // used to store number of vertices when gone to the gpu
  
  // Member functions - private
};

#endif




