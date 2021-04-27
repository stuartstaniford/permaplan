// Copyright Staniford Systems.  All Rights Reserved.  June 2020 -
// This class is intended to assemble buffers of vertices and associated
// EBO indices for ultimate drawing with glDrawElements.  The model is
// that one can declare a large such buffer, put many distinct objects into
// it, and then pass them all to the GPU.


#include "Shader.h"
#include "TriangleBuffer.h"
#include <err.h>
#include <assert.h>


// =======================================================================================
// Constructor allocates the buffers for both vertices and indices into the vertex array

TriangleBuffer::TriangleBuffer(unsigned vertexCount, unsigned indexCount, char* name):
                                  vCount(vertexCount),
                                  iCount(indexCount),
                                  vNext(0u),
                                  iNext(0u),
                                  combo(NULL),
                                  bufName(name)
{
  //fprintf(stderr, "Triangle buffer of size %d,%d allocated\n", vCount, iCount);
  vertices = new Vertex[vCount];
  unless(vertices)
    err(-1, "Couldn't allocate vertex space in TriangleBuffer::TriangleBuffer");
  indices = new unsigned[iCount];

  unless(indices)
    err(-1, "Couldn't allocated index space in TriangleBuffer::TriangleBuffer");
  LogTriangleBufferOps("New TriangleBuffer %s sizes: [%u, %u]\n",
                                                    bufName, vCount, iCount);

  incrementTriangleBufferMemory(sizeof(TriangleBuffer) + vCount*sizeof(Vertex)
                                                        + iCount*sizeof(unsigned));
}


// =======================================================================================
// Destructor deallocates the buffers

TriangleBuffer::~TriangleBuffer(void)
{
  unless(vNext == vCount && iNext == iCount)
    LogTriangleBufferErrs("Deallocating partially used TriangleBuffer %s: "
                                                    "[%u, %u]/[%u,%u].\n",
                                                    bufName, vNext, iNext, vCount, iCount);
  
  if(vertices)
   {
    delete[] vertices;
    incrementTriangleBufferMemory(-vCount*sizeof(Vertex));
   }
  if(indices)
   {
    delete[] indices;
    incrementTriangleBufferMemory(-iCount*sizeof(unsigned));
  }
  if(combo)
   {
    delete combo;
    incrementTriangleBufferMemory(-sizeof(ElementBufferCombo));
   }
  incrementTriangleBufferMemory(-sizeof(TriangleBuffer));
}


// =======================================================================================
// This provides service to visualObjects that need to add their geometry into this
// TriangleBuffer, giving them a slice of both buffers that they can use

bool TriangleBuffer::requestSpace(Vertex** verticesAssigned, unsigned** indicesAssigned,
                  unsigned& vOffset, unsigned vRequestCount, unsigned iRequestCount)
{
  unless(vertices && indices)
   err(-1, "Trying to request space on unallocated TriangleBuffer\n");

#ifdef LOG_TRIANGLE_BUFFER_ERRS
  if(iRequestCount%3)
   {
    LogTriangleBufferErrs("Fractional TriangleBuffer %s request: iCount: %u\n",
                                                              bufName, iRequestCount);
    return false;
   }
  if(vRequestCount > iRequestCount)
   {
    LogTriangleBufferErrs("TriangleBuffer %s, vCount: %u greater than iCount: %u\n",
                                            bufName, vRequestCount, iRequestCount);
    return false;
   }
#endif
  
  if(vNext + vRequestCount <= vCount && iNext + iRequestCount <= iCount)
   {
    *verticesAssigned =  vertices + vNext;
    *indicesAssigned  =  indices + iNext;
    vOffset           =  vNext;
    vNext             += vRequestCount;
    iNext             += iRequestCount;
    LogTriangleBufferOps("Successful TriangleBuffer %s request: [%u, %u] now "
                         "[%u, %u] of [%u, %u]\n", bufName,
                         vRequestCount, iRequestCount,
                         vNext, iNext, vCount, iCount);
    return true;
   }
  else
   {
    LogTriangleBufferErrs("Failed TriangleBuffer %s request: vCount: %u, iCount: %u\n",
                                        bufName, vRequestCount, iRequestCount);
    return false;
   }
}


// =======================================================================================
// Sets up the appropriate VBO, VAO, and EBO, and dispatches the data.  This is called
// only after the TriangleBuffer has been assembled.

void TriangleBuffer::sendToGPU(GLenum usage)
{
  if(combo)
   {
    delete combo;
    incrementTriangleBufferMemory(-sizeof(ElementBufferCombo));
   }
  combo = new ElementBufferCombo(vertices, vCount, indices, iCount, usage);
  incrementTriangleBufferMemory(sizeof(ElementBufferCombo));
#ifdef LOG_VALID_TRIANGLE_BUFS
  selfValidate();
#endif
  glBufferData(GL_ARRAY_BUFFER, vCount*sizeof(Vertex), vertices, usage);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, iCount*sizeof(unsigned), indices, usage);
  delete[] vertices;
  delete[] indices;
  incrementTriangleBufferMemory(-vCount*sizeof(Vertex) - iCount*sizeof(unsigned));
  vertices = NULL;
  indices = NULL;
}


// =======================================================================================
// Function which will run through all our vertices/indices and sanity check everything.
// This is done right before dispatch to the GPU so the buffer should be in a sane 
// condition.

#ifdef LOG_VALID_TRIANGLE_BUFS

void TriangleBuffer::selfValidate(void)
{
  // Buffer should be exactly full
  unless(vNext == vCount)
    goto BadExit;  
  unless(iNext == iCount)
    goto BadExit;
  
  // indices should be in groups of 3.
  unless(!(iNext%3))
    goto BadExit;
  
  for(int i=0; i<iNext; i+=3)
   {
    for(int j=0; j<3; j++)
     {
      // triangles should point to valid vertices
      unless(indices[i+j] < vNext)
        goto BadExit;
 
      // triangles should not cross objects
      if(j>0)
       {
        unless(vertices[indices[i+j]].objectId == vertices[indices[i]].objectId)
          goto BadExit;
       }
     }
   }
                                                          
  // XX should sanity check vertices

  return;  // exit function here if all is well
  
BadExit:      // stuff to do when we have failed a test
  
  dumpBuffer();
  assert(0);
}
#endif


// =======================================================================================
// Bind our OpenGL objects and then render our objects

void TriangleBuffer::draw(VertexDrawType drawType, vec4 objColor)
{
  unless(combo)
    err(-1, "No combo in TriangleBuffer::draw");
  combo->bind();
  Shader& shader = Shader::getMainShader();
  if(drawType == FixedColor)
   {
    shader.setUniform("fixedColor", true);
    shader.setUniform("theColor", objColor);
   }
  else if(drawType == NoTexColor)
    shader.setUniform("noTexColor", true);

  glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, 0);
  shader.setUniform("fixedColor", false);
  shader.setUniform("noTexColor", false);
  
  if(checkGLError(stderr, "TriangleBuffer::draw"))
    exit(-1);
}


// =======================================================================================
// Dump our state to an HTML file for debugging/diagnostic purposes- called from 
// selfValidate().

void TriangleBuffer::dumpBuffer(void)
{
  FILE* file = fopen("dumpTriangleBuf.html", "w");
  assert(file);
  
  // Output all the vertices
  for(int v = 0; v < vCount; v++)
   {
    Vertex* b = vertices + v;
    fprintf(file, "%d\txyz: %.1f %.1f %.1f\tst: %.3f, %.3f\tcolor: %X\n",
             v, b->pos[0], b->pos[1], b->pos[2], b->tex[0], b->tex[1], b->color);
   }
  
  // Output all the indices, one triangle per row
  for(int i = 0; i < iCount; i+=3)
   {
    fprintf(file, "%d:\t",i/3);
    for(int j = 0; j < 3; j++)
      fprintf(file,"%d ", indices[i+j]);
    fprintf(file, "\n");
   }
  
  fflush(file);
}


// =======================================================================================
// Dump our state to a file in an ASCII form for debugging (this is used when the
// state is transient and cannot easily be inspected with diagnosticHTML();

void TriangleBuffer::fprint(FILE* file)
{
  // Output all the vertices
  for(int v = 0; v < vCount; v++)
   {
    Vertex* b = vertices + v;
    fprintf(file, "%d\txyz: %.1f %.1f %.1f\tst: %.3f, %.3f\tcolor: %X\n",
             v, b->pos[0], b->pos[1], b->pos[2], b->tex[0], b->tex[1], b->color);
   }
  
  // Output all the indices, one triangle per row
  for(int i = 0; i < iCount; i+=3)
   {
    fprintf(file, "%d:\t",i/3);
    for(int j = 0; j < 3; j++)
      fprintf(file,"%d ", indices[i+j]);
    fprintf(file, "\n");
   }
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
  httPrintf("<tr><th>Index</th><th>X</th><th>Y</th><th>Z</th>");
  httPrintf("<th>S</th><th>T</th><th>Color</th></tr>\n");
  for(int v = 0; v < vCount; v++)
   {
    httPrintf("<tr>");
    
    // Index
    httPrintf("<td><a name = %d>%d</td>", v,v);
    
    // X,Y,Z
    for(int i = 0; i < 3; i++)
      httPrintf("<td>%.1f</td>", vertices[v].pos[i]);

    // S,T, accent
    for(int i = 4; i <= 5; i++)
      httPrintf("<td>%.4f</td>", vertices[v].tex[i]);
    httPrintf("<td>%X</td>", vertices[v].color);

    httPrintf("</tr>\n");
   }
  httPrintf("</table></center>\n");

  // Table of the indices we store in rows of three, one row per triangle
  serv->newSection("Triangles");
  serv->startTable();
  httPrintf("<tr><th>Index</th><th>1st</th><th>2nd</th><th>3rd</th></tr>\n");
  for(int i = 0; i < iCount; i+=3)
   {
    httPrintf("<tr>");
    httPrintf("<td>%d</td>", i/3);
    for(int j = 0; j < 3; j++)
    httPrintf("<td><a href=\"#%d\">%d</a></td>", indices[i+j], indices[i+j]);
    httPrintf("</tr>\n");
   }
  httPrintf("</table></center>\n");

  return true;
}


// =======================================================================================
