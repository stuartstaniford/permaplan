// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Display a set of colored lines.  This is primarily used for visual indicators 
// (axes, grids, rays, etc), rather than actual scene objects.  It's displayed 
// via a call to glDrawArrays(GL_LINES) rather than being triangles in a 
// TriangleBuffer.

#include "LineStripList.h"
#include "VertexBufferCombo.h"
#include "Shader.h"
#include "HttpDebug.h"


// =======================================================================================
/// @brief Constructor
/// 
/// This just sets us up as an empty vector.  Subsequently, line segments should be
/// added to taste via addLine, and then the data must be sent to the gpu with sendToGpu.  
/// Only then can it be rendered via draw.
/// @param usage A GLenum to be passed to glBufferData specifying the usage of the data
/// see (man 3 glBufferData).

LineStripList::LineStripList(GLenum usage):
                        std::vector<Vertex>(),
                        combo(NULL),
                        usageGL(usage),
                        N(0u)
{
}


// =======================================================================================
/// @brief Destructor

LineStripList::~LineStripList(void)
{
  if(combo)
    delete combo;
}


// =======================================================================================
/// @brief Add an additional line on the end of the buffer.
/// @param pos The vec3 starting point of the line segment.
/// @param dir The vec3 vector from the start to the end of the line segment.
/// @param color The color that this line segment should be drawn in.

void LineStripList::addLine(vec3 pos, vec3 dir, unsigned color)
{
  N = this->size();
  this->resize(N+2);
  this->at(N).setPosition(pos);
  this->at(N).setColor(color);
  vec3 otherEnd;
  glm_vec3_add(pos, dir, otherEnd);
  this->at(N+1).setPosition(otherEnd);
  this->at(N+1).setColor(color);
  N+=2;
}


// =======================================================================================
/// @brief Sets up the appropriate VBO, VAO, and dispatches the data to the graphics card.
/// 
/// This should only be called when the buffer is new, or has changed.

void LineStripList::sendToGPU(void)
{
  if(combo)
    delete combo;
  combo = new VertexBufferCombo(this->size(), this->data(), usageGL);
  this->resize(0);
}


// =======================================================================================
/// @brief Renders on screen.  
/// 
/// Will not work unless sendToGPU has been called earlier.

void LineStripList::draw(void)
{
  Shader& shader = Shader::getMainShader();
  shader.setUniform("noTexColor", true);
  combo->bind();
  glDrawArrays(GL_LINES, 0, N);
  shader.setUniform("noTexColor", false);
  if(checkGLError(stderr, "LineStripList::draw"))
    exit(-1);
}


// =======================================================================================
/// @brief Provide diagnostic HTML 
/// @todo Not implemented

bool LineStripList::diagnosticHTML(HttpDebug* serv)
{

  return false;
}


// =======================================================================================
