// Copyright Staniford Systems.  All Rights Reserved.  June 2020 -
// This class is intended to assemble buffers of vertices and associated
// EBO indices for ultimate drawing with glDrawElements.  The model is
// that one can declare a large such buffer, put many distinct objects into
// it, and then pass them all to the GPU.


#include <err.h>
#include "Shader.h"
#include <TriangleBuffer.h>


// =======================================================================================
// Constructor allocates the buffers for both vertices and indices into the vertex array

TriangleBuffer::TriangleBuffer(unsigned vertexCount, unsigned indexCount):
                vCount(vertexCount), iCount(indexCount), vNext(0u), iNext(0u), combo(NULL)
{
  vertices = new VertexBufElement[vCount];
  unless(vertices)
    err(-1, "Couldn't allocate vertex space in TriangleBuffer::TriangleBuffer");
  indices = new unsigned[iCount];

  unless(indices)
    err(-1, "Couldn't allocated index space in TriangleBuffer::TriangleBuffer");
}


// =======================================================================================
// Destructor deallocates the buffers

TriangleBuffer::~TriangleBuffer(void)
{
  if(vertices)
    delete[] vertices;
  if(indices)
    delete[] indices;
  if(combo)
    delete combo;
}


// =======================================================================================
// This provides service to visualObjects that need to add their geometry into this
// TriangleBuffer, giving them a slice of both buffers that they can use

bool TriangleBuffer::requestSpace(VertexBufElement** verticesAssigned, unsigned** indicesAssigned,
                  unsigned& vOffset, unsigned& iOffset,
                  unsigned vRequestCount, unsigned iRequestCount)
{
  unless(vertices && indices)
   err(-1, "Trying to request space on unallocated TriangleBuffer\n");
  if(vNext + vRequestCount <= vCount && iNext + iRequestCount <= iCount)
   {
    *verticesAssigned =  vertices + vNext;
    *indicesAssigned  =  indices + iNext;
    vOffset           =  vNext;
    iOffset           =  iNext;
    vNext             += vRequestCount;
    iNext             += iRequestCount;
    return true;
   }
  else
    return false;
}


// =======================================================================================
// Sets up the appropriate VBO, VAO, and EBO, and dispatches the data.  This is called
// only after the TriangleBuffer has been assembled.

void TriangleBuffer::sendToGPU(GLenum usage)
{
  if(combo)
    delete combo;
  combo = new ElementBufferCombo(vertices, vCount, indices, iCount, usage);
  glBufferData(GL_ARRAY_BUFFER, vCount*sizeof(VertexBufElement), vertices, usage);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount*sizeof(unsigned), indices, usage);
  delete[] vertices;
  delete[] indices;
  vertices = NULL;
  indices = NULL;
}


// =======================================================================================
// Bind our OpenGL objects and then render our objects

void TriangleBuffer::draw(void)
{
  unless(combo)
    return;
  combo->bind();
  glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, 0);
  if(checkGLError(stderr, "TriangleBuffer::draw"))
    exit(-1);

}


// =======================================================================================
// Provide diagnostic html about the triangles in our buffer.  Since we don't really
// know anything about where we are located or what our contents mean, we don't
// provide page header/footer, but rather just two tables, one of our vertices, and
// one of the triangles in the indices array

bool TriangleBuffer::diagnosticHTML(HttpDebug* serv)
{
  // Table of the vertices we store
  unless(vertices && indices)
    return false;
  serv->newSection("Vertices");
  serv->startTable();
  serv->addResponseData("<tr><th>Index</th><th>X</th><th>Y</th><th>Z</th>");
  serv->addResponseData("<th>S</th><th>T</th><th>Accent</th></tr>\n");
  for(int v = 0; v < vCount; v++)
   {
    serv->addResponseData("<tr>");
    
    // Index
    serv->respPtr += sprintf(serv->respPtr, "<td><a name = %d>%d</td>", v,v);
    
    // X,Y,Z
    for(int i = 0; i < 3; i++)
      serv->respPtr += sprintf(serv->respPtr, "<td>%.1f</td>", vertices[v].pos[i]);

    // S,T, accent
    for(int i = 4; i <= 5; i++)
      serv->respPtr += sprintf(serv->respPtr, "<td>%.4f</td>", vertices[v].tex[i]);
    serv->respPtr += sprintf(serv->respPtr, "<td>%.4f</td>", vertices[v].accent);

    serv->addResponseData("</tr>\n");
   }
  serv->addResponseData("</table></center>\n");

  // Table of the indices we store in rows of three, one row per triangle
  serv->newSection("Triangles");
  serv->startTable();
  serv->addResponseData("<tr><th>Index</th><th>1st</th><th>2nd</th><th>3rd</th></tr>\n");
  for(int i = 0; i < iCount; i+=3)
   {
    serv->addResponseData("<tr>");
    serv->respPtr += sprintf(serv->respPtr, "<td>%d</td>", i/3);
    for(int j = 0; j < 3; j++)
      serv->respPtr += sprintf(serv->respPtr, "<td><a href=\"#%d\">%d</a></td>",
                               indices[i+j], indices[i+j]);
    serv->addResponseData("</tr>\n");

   }
  serv->addResponseData("</table></center>\n");

  return true;
}


// =======================================================================================
