// Copyright Staniford Systems.  All Rights Reserved.  June 2020 -
// This class is intended to assemble buffers of vertices and associated
// ebo indices for ultimate drawing with glDrawElements.  The model is
// that one can declare a large such buffer, put many distinct objects into
// it, and then pass them all to the GPU.


#ifndef TRIANGLE_BUFFER_H
#define TRIANGLE_BUFFER_H

#include "ElementBufferCombo.h"
#include "HttpDebug.h"


// =======================================================================================
// C++ wrapper around the openGL Vertex Buffer Object

class TriangleBuffer
{
 public:
  
  // Instance variables - public
  
  // Member functions - public
  TriangleBuffer(unsigned vertexCount, unsigned indexCount);
  ~TriangleBuffer(void);
  bool requestSpace(VertexBufElement** verticesAssigned, unsigned** indicesAssigned,
                    unsigned& vOffset, unsigned& iOffset,
                    unsigned vRequestCount, unsigned iRequestCount);
  void sendToGPU(GLenum usage);
  void draw(void);
  bool diagnosticHTML(HttpDebug* serv);
  
 private:
  
  // Instance variables - private
  unsigned*             indices;
  VertexBufElement*     vertices;
  unsigned              vCount;
  unsigned              iCount;
  unsigned              vNext;
  unsigned              iNext;
  ElementBufferCombo*   combo;

  // Member functions - private
  
  
};

#endif
