
// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Class that contains a list of visual objects and some methods for managing
// the list as a whole.  Handy for quickly throwing up a set of something
// or others for display.

#include "LineStripList.h"


// =======================================================================================
// Constructors

LineStripList::LineStripList(Shader& S, GLenum usage):
                        std::vector<VertexBufElement>(),
                        shader(S),
                        combo(NULL),
                        usageGL(usage),
                        N(0u)
{
}


// =======================================================================================
// Destructor

LineStripList::~LineStripList(void)
{
  if(combo)
    delete combo;
}


// =======================================================================================
// Add an additional line on the end of the buffer

void LineStripList::addLine(vec3 pos, vec3 dir, vec3 color)
{
  N = this->size();
  this->resize(N+2);
  this->at(N).set(pos, color);
  vec3 otherEnd;
  glm_vec3_add(pos, dir, otherEnd);
  this->at(N+1).set(otherEnd, color);
  N+=2;
}


// =======================================================================================
// Sets up the appropriate VBO, VAO, and dispatches the data.  Only call when the buffer
// is new, or has changed.

void LineStripList::sendToGPU(void)
{
  if(combo)
    delete combo;
  combo = new VertexBufferCombo(this->size(), this->data(), usageGL);
  this->resize(0);
  this->shrink_to_fit();
}


// =======================================================================================
// Renders on screen.  Will not work unless bufferGeometry has been called earlier.

void LineStripList::draw(void)
{
  shader.setUniform("noTexColor", true);
  combo->bind();
  glDrawArrays(GL_LINES, 0, N);
  shader.setUniform("noTexColor", false);
  if(checkGLError(stderr, "LineStripList::draw"))
    exit(-1);
}


// =======================================================================================
// Not implemented

bool LineStripList::diagnosticHTML(HttpDebug* serv)
{

  return false;
}


// =======================================================================================
