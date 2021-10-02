// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -

#ifndef LINE_STRIP_LIST_H
#define LINE_STRIP_LIST_H

#include <vector>
#include <cglm/cglm.h>
#include <GL/glew.h>
#include "Vertex.h"


// =======================================================================================
// Forward declarations

class VertexBufferCombo;
class Vertex;
class HttpDebug;


// =======================================================================================
/// @brief Display a set of colored lines.
/// 
/// This is primarily used for visual indicators (axes, grids, rays, etc), rather than
/// actual scene objects.  It's displayed via a call to glDrawArrays(GL_LINES) rather
/// than being triangles in a TriangleBuffer.  We inherit from a vector of Vertex
/// instances, which is taken in disjoint pairs to represent each successive line
/// segment to be drawn. 

class LineStripList: public std::vector<Vertex>
{
 public:
  
  // Instance variables - public

  // Member functions - public
  LineStripList(GLenum usage = GL_STATIC_DRAW);
  ~LineStripList();
  void addLine(vec3 pos, vec3 dir, unsigned color);
  void sendToGPU(void);
  void draw(void);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  VertexBufferCombo*  combo;
  GLenum              usageGL;
  unsigned            N;       // used to store number of vertices when gone to the gpu
  
  // Member functions - private
  LineStripList(const LineStripList&);                 // Prevent copy-construction
  LineStripList& operator=(const LineStripList&);      // Prevent assignment
};


// =======================================================================================

#endif




