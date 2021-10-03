// Copyright Staniford Systems.  All Rights Reserved.  June 2020 -

#ifndef TRIANGLE_BUFFER_H
#define TRIANGLE_BUFFER_H

#include "Vertex.h"
#include <err.h>
#include <GL/glew.h>


// =======================================================================================
// Forward declarations

class ElementBufferCombo;
class HttpDebug;


// =======================================================================================
/// @brief Class for buffers of triangles for rendering.
/// 
/// This class is intended to assemble buffers of vertices (representing triangles of
/// Vertex) and associated ebo indices for ultimate drawing with glDrawElements.  The 
/// model is that one can declare a large such buffer, put many distinct objects into it, 
/// and then pass them all to the GPU..

class TriangleBuffer
{
  friend void recycleTriangleBuffer(TriangleBuffer*& tbuf, int vCount, int iCount);
 public:
  
  // Instance variables - public
  unsigned              vCount;
  unsigned              iCount;
  char*                 bufName;

  // Member functions - public
  TriangleBuffer(unsigned vertexCount, unsigned indexCount, char* name);
  ~TriangleBuffer(void);
  bool requestSpace(Vertex** verticesAssigned, unsigned** indicesAssigned,
                    unsigned& vOffset, unsigned vRequestCount, unsigned iRequestCount);
  void sendToGPU(GLenum usage);
  void recreateInNewContext(void);
  void draw(VertexDrawType drawType, vec4 objColor);
  bool sanityCheckPosition(unsigned v);
  void selfValidate(void);
  void dumpBuffer(void);
  void fprint(FILE* file);
  bool diagnosticHTML(HttpDebug* serv);
  
 private:
  
  // Instance variables - private
  unsigned*             indices;
  Vertex*               vertices;
  unsigned              vNext;
  unsigned              iNext;
  ElementBufferCombo*   combo;

  // Member functions - private
  TriangleBuffer(const TriangleBuffer&);                 // Prevent copy-construction
  TriangleBuffer& operator=(const TriangleBuffer&);      // Prevent assignment
};


// =======================================================================================
// Helper functions

// =======================================================================================
/// @brief Function used when we need to throw away a triangle buffer and replace it.
/// 
/// This is still a common pattern in the code when a buffer needs to be recycled.
///
/// @param tbuf A reference to a pointer to the buffer
/// @param vCount The number of instances of Vertex required in the reallocated buffer.
/// @param iCount The number of indices required in the reallocated buffer.
/// @param name The name of this buffer (mainly for logging/diagnostic purposes).
/// 
/// @todo The existence of this seems inefficient - is there a better way?

inline void recycleTriangleBuffer(TriangleBuffer*& tbuf, int vCount, int iCount, char* name)
{
  if(tbuf)
    delete tbuf; //triangleBufferMemory //triangleBufferMemory
  tbuf = new TriangleBuffer(vCount, iCount, name);
  if(!tbuf)
    err(-1, "Can't allocate memory in __func__\n");
  //fprintf(stderr, "Triangle buffer of size %d,%d obtained\n", tbuf->vCount, tbuf->iCount);
}

#endif
