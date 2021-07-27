// Copyright Staniford Systems.  All Rights Reserved.  June 2020 -
// This class is intended to assemble buffers of vertices and associated
// EBO indices for ultimate drawing with glDrawElements.  The model is
// that one can declare a large such buffer, put many distinct objects into
// it, and then pass them all to the GPU.


#include "TriangleBuffer.h"
#include "Shader.h"
#include "PmodDesign.h"
#include "BoundingBox.h"
#include <err.h>
#include <assert.h>


// =======================================================================================
/// @brief The constructor allocates the buffers for both vertices and indices into the
/// respective arrays.
///
/// @param vertexCount The number of instances of Vertex to be allocated - the total
/// vertex capacity of the buffer.
/// @param indexCount The total number of indices to be allocated in the buffer.
/// @param name The name of this buffer (mainly for logging/diagnostic purposes).

TriangleBuffer::TriangleBuffer(unsigned vertexCount, unsigned indexCount, char* name):
                                  vCount(vertexCount),
                                  iCount(indexCount),
                                  bufName(name),
                                  vNext(0u),
                                  iNext(0u),
                                  combo(NULL)
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
// @brief Destructor deallocates the buffers and the ElementBufferCombo

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
/// @brief The main API by which VisualObject and VisualElement subtypes request a slice
/// of both buffers.
/// 
/// This allows them to add their geometry into this TriangleBuffer.  Once all relevant
/// objects have been assembled into the buffer, it can be sent to the GPU with 
/// sendToGPU.
///
/// @returns True if all went well, false in the case of something going wrong with the
/// request.
/// @param verticesAssigned A pointer to a pointer to the location in the vertex array
/// that the caller can use.
/// @param indicesAssigned A pointer to a pointer to the location in the index array
/// that the caller can use.
/// @param vOffset A reference to an unsigned used to hold the correct vertex offset
/// that the caller should base all indices they write into the index array (since the
/// caller wouldn't otherwise know where in the vertex array their vertices are being
/// put.
/// @param vRequestCount The count of Vertex space being requested.
/// @param iRequestCount The count of index space being requested.

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
/// @brief Sends the all the current data in the buffers to the GPU.
///
/// Sets up the appropriate VBO, VAO, and EBO, and dispatches the data.  This should be
/// called only after the TriangleBuffer has been assembled.  After this, the vertex
/// and index arrays will be deleted, and recycleTriangleBuffer() can be used to refresh 
/// the buffer for reuse.

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
/// @brief Function which sanity checks a particular vertex.
///
/// Is a helper to selfValidate().
/// @returns True if all is well, false if a problem is detected
/// @param v The offset in the vertex array to check.

bool TriangleBuffer::sanityCheckPosition(unsigned v)
{
  PmodDesign& design = PmodDesign::getDesign();
  unless(design.designBoxValid)
    return true;
  BoundingBox& B = design.designBox;
  vec3& point = vertices[v].pos;

  
  if(point[0] < B.lower[0])
   {
    LogValidTriangleBufs("TriangleBuffer::selfValidate: vertex %u; X val %.1f is "
                         "below %.1f.\n", v, point[0], B.lower[0]);
    return false;
   }
  if(point[0] > B.upper[0])
   {
    LogValidTriangleBufs("TriangleBuffer::selfValidate: vertex %u; X val %.1f is "
                         "above %.1f.\n", v, point[0], B.upper[0]);
    return false;
   }
  if(point[1] < B.lower[1])
   {
    LogValidTriangleBufs("TriangleBuffer::selfValidate: vertex %u; Y val %.1f is "
                         "below %.1f.\n", v, point[1], B.lower[1]);
    return false;
   }
  if(point[1] > B.upper[1])
   {
    LogValidTriangleBufs("TriangleBuffer::selfValidate: vertex %u; Y val %.1f is "
                         "above %.1f.\n", v, point[1], B.upper[1]);
    return false;
   }
  if(point[2] < B.lower[2])
   {
    LogValidTriangleBufs("TriangleBuffer::selfValidate: vertex %u; Z val %.1f is "
                         "below %.1f.\n", v, point[2], B.lower[2]);
    return false;
   }
  if(point[2] > B.upper[2])
   {
    LogValidTriangleBufs("TriangleBuffer::selfValidate: vertex %u; Z val %.1f is "
                         "above %.1f.\n", v, point[2], B.upper[2]);
    return false;
   }
  return true;
}


// =======================================================================================
/// @brief Function which will run through all our vertices/indices and sanity check 
/// everything looking for a variety of possible error conditions.
///
/// This is done right before dispatch to the GPU so the buffer should be in a sane 
/// condition.


void TriangleBuffer::selfValidate(void)
{
#ifdef LOG_VALID_TRIANGLE_BUFS
  // Buffer should be exactly full
  unless(vNext == vCount)
   {
    LogValidTriangleBufs("TriangleBuffer::selfValidate: buffer vertices not fully "
                                                                            "utilized.\n");
    goto BadExit;
   }
  unless(iNext == iCount)
   {
    LogValidTriangleBufs("TriangleBuffer::selfValidate: buffer indices not fully "
                                                                            "utilized.\n");
    goto BadExit;
   }
  
  // indices should be in groups of 3.
  unless(!(iNext%3))
   {
    LogValidTriangleBufs("TriangleBuffer::selfValidate: iNext has incomplete triangle.\n");
    goto BadExit;
   }
  
  for(int i=0; i<iNext; i+=3)
   {
    for(int j=0; j<3; j++)
     {
      // triangles should point to valid vertices
      unless(indices[i+j] < vNext)
      {
       LogValidTriangleBufs("TriangleBuffer::selfValidate: index %u references "
                                                  "invalid vertex %u\n", i+j, indices[i+j]);
       goto BadExit;
      }
 
      // triangles should not cross objects
      if(j>0)
       {
        unless(vertices[indices[i+j]].objectId == vertices[indices[i]].objectId)
         {
          LogValidTriangleBufs("TriangleBuffer::selfValidate: triangle %u crosses objects "
                                  "%u and %u.\n", i/3, vertices[indices[i+j]].objectId,
                                  vertices[indices[i]].objectId);
          goto BadExit;
         }
       }
     }
   }
   
  // Check vertices
  for(int v=0; v < vNext; v++)
   {
    unless(sanityCheckPosition(v))
     {
      LogValidTriangleBufs("TriangleBuffer::selfValidate vertex %d/[0..%d] failed "
                                                                "sanity check", v, vNext-1);
      goto BadExit;
     }
   }
  
  
  return;  // exit function here if all is well
  
BadExit:      // stuff to do when we have failed a test
  
  LogFlush();
  dumpBuffer();
  assert(0);
#endif
}


// =======================================================================================
/// @brief Render the objects in the TriangleBuffer.
///
/// Function to bind our OpenGL objects and then render the objects that were put into
/// our buffers.  Note this can only be done after the call to sendToGPU.
///
/// @param drawType A VertexDrawType specifying which kind of rendering is to be done.
/// @param objColor A vec4 for the color to make everything (only used if drawType
/// is set to FixedColor.

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
/// @brief Dump our state to an HTML file for debugging/diagnostic purposes.
/// 
/// Called from selfValidate().

void TriangleBuffer::dumpBuffer(void)
{
  FILE* file = fopen("dumpTriangleBuf.html", "w");
  assert(file);
  
  fprintf(file, "<html><head><title>Triangle Buffer Dump of %s</title></head>\n", bufName);
  fprintf(file, "<body><h1>Triangle Buffer Dump of %s</h1>\n", bufName);
  fprintf(file, "<b>vCount (used):</b> %u (%u)<br>\n", vCount, vNext);
  fprintf(file, "<b>iCount (used):</b> %u (%u)<br>\n", iCount, iNext);

  // Output all the vertices in a table
  fprintf(file, "<h2>Table of Vertices</h2>\n");
  fprintf(file, "<table border = 1 cellpadding = 1>\n");
  Vertex::printVertexTableHeader(file);
  for(unsigned v = 0; v < vNext; v++)
    vertices[v].printVertexTableRow(file, v);
  fprintf(file, "</table>\n");
  
  // Output all the indices, one triangle per row
  fprintf(file, "<h2>Table of Triangles/Indices</h2>\n");
  fprintf(file, "<table border = 1 cellpadding = 1>\n");
  fprintf(file, "<tr><th>Triangle</th><th>Vertex indices</th>\n");
  fprintf(file, "<th>Object Ids</th></tr>\n");
  for(int i = 0; i < iNext; i+=3)
   {
    fprintf(file, "<tr><td>%d</td><td>",i/3);
    for(int j = 0; j < 3; j++)
      fprintf(file,"<a href=\"#row%u\">%u</a> ", indices[i+j], indices[i+j]);
    fprintf(file, "</td><td>");
    for(int j = 0; j < 3; j++)
      fprintf(file,"%u ", vertices[indices[i+j]].objectId);            
    fprintf(file, "</td></tr>\n");
   }
  fprintf(file, "</table>\n");

  // Close out the page and file
  fprintf(file, "</body></html>\n");
  fflush(file);
  fclose(file);
}


// =======================================================================================
/// @brief Dump our state to a file in an ASCII form for debugging.
///  
/// This is used when the state is transient and cannot easily be inspected with 
/// diagnosticHTML();

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
/// @brief Provide diagnostic html to the HTTP interface about the triangles in our buffer.  
/// 
/// Since we don't really know anything about where we are located or what our contents 
/// mean, we don't provide page header/footer, but rather just two tables, one of our 
/// vertices, and one of the triangles in the indices array.
///
/// @returns True if everything went well, false if we couldn't fit in the buffer.
/// @param serv The HttpDebug server instance to talk to.

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
