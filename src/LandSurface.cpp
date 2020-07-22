// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// Class which loads a series of records of surface height at various locations
// and produces a tiling of the surface, as well as being able to texture it
// with a provided map.

#include <cstdio>
#include <stdexcept>
#include <err.h>
#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "LandSurface.h"


// =======================================================================================
// Constructor which sets up the surface as specified in the design file

LandSurface::LandSurface(Shader& S, PmodDesign& D): rect(NULL), qtree(NULL), shader(S),
                                          design(D),
#ifdef USE_TRIANGLE_BUFFER
                                          tbuf(NULL),
#else
                                          VAOs(1),
#endif
                                            locationCount(0u)
{
  using namespace rapidjson;

  if(!(design.doc.HasMember("landSurface") && design.doc["landSurface"].IsObject()))
    err(-1, "No land surface available\n");
  
  Value& LsJson = design.doc["landSurface"];
  float width;
  if(LsJson.HasMember("width") && LsJson["width"].IsNumber())
    width = LsJson["width"].GetFloat();
  else
    err(-1, "Bad landSurface width in file %s\n", design.config.designFileName);
  if(LsJson.HasMember("textureFile") && LsJson["textureFile"].IsString())
    rect = new TexturedRect(shader, LsJson["textureFile"].GetString(), width, 0.0f);
  else
    err(-1, "Bad landSurface texturefile in file %s\n", design.config.designFileName);

  if(checkGLError(stderr, "LandSurface::LandSurface"))
    exit(-1);
}


// =======================================================================================
// Late constructor stuff that can't be done until after the Qtree is initialized

void LandSurface::bufferGeometry(Quadtree* q)
{
  qtree = q;
  
#ifdef USE_TRIANGLE_BUFFER
  if(tbuf)
    delete tbuf;
  //XXX note sizing here is inefficient, needs to be replaced with more dynamic
  // approach
  tbuf = new TriangleBuffer(qtree->landVBOSize, qtree->landVBOSize);
  if(!tbuf)
    err(-1, "Can't allocate memory in __func__\n");
  qtree->bufferLandSurface(tbuf);
  tbuf->sendToGPU(GL_STATIC_DRAW);
#else
  VertexBufElement* buf = new VertexBufElement[qtree->landVBOSize];
  if(!buf)
    err(-1, "Can't allocate memory in __func__\n");
  qtree->bufferGeometry(buf);
  VAOs.bind(0);
  VBO = new VertexBufferObject(qtree->landVBOSize, buf, GL_DYNAMIC_DRAW);
  delete[] buf;
#endif
  
}


// =======================================================================================
// Destructor

LandSurface::~LandSurface(void)
{
  if(rect)
    delete rect;

#ifdef USE_TRIANGLE_BUFFER
  if(tbuf)
    delete tbuf;
#else
  if(VBO)
    delete VBO;
#endif
}


// =======================================================================================
// Highlight one qtree node.  If color is NULL, unhighlights.  Assumes the correct
// VAO/VBO are already bound

void LandSurface::highlightNode(Quadtree* targetNode, vec4& color, float accent)
{
#ifdef USE_TRIANGLE_BUFFER
  
#else
  //printf("Size: %u\tOffset: %u\n", targetNode->landVBOSize, targetNode->bufferOffset);
  VertexBufElement* buf = (VertexBufElement*)glMapBufferRange(GL_ARRAY_BUFFER,
                        (targetNode->bufferOffset)*sizeof(VertexBufElement),
                        (targetNode->landVBOSize)*sizeof(VertexBufElement), GL_MAP_WRITE_BIT);
  if(checkGLError(stderr, "LandSurface::highlightNode:glMapBufferRange") || !buf)
    exit(-1);
  VertexBufElement* b;
  int i;
  if(accent > 0.0f)
    shader.setUniform("accentColor", color);
  for(b = buf, i=0; i<targetNode->landVBOSize;i++,b++)
      b->accent = accent;

  glUnmapBuffer(GL_ARRAY_BUFFER);
  if(checkGLError(stderr, "LandSurface::highlightNode"))
    exit(-1);
#endif
}


// =======================================================================================
// Reconfigure the land surface when we are told about a new observation that is to be
// added to our state

extern vec3 zAxis;

void LandSurface::newLandHeight(HeightMarker* hM)
{
  locationCount++;
  vec3 plane;
  
  if(locationCount == 1)
   {
    // level surface at the new height
    plane[0] = 0.0f; // co-efficient of x
    plane[1] = 0.0f; // co-efficient of y
    plane[2] = hM->location[2]; // intersection with the z-axis
   }
  else if(locationCount == 2)
   {
    // includes both points, sloping up
    //float *lower, *upper;
    plane[0] = plane[1] = plane[2] = 0.0f; //XXX stub
   }
  else if(locationCount == 3)
   {
    // plane through all three points
    plane[0] = plane[1] = plane[2] = 0.0f; //XXX stub
   }
  
  // store pointer to hM in our table

  if(locationCount <= 3) // done already
   {
    qtree->redoLandPlanar(plane);
    return;
   }
  
  // Otherwise get into doing some math with all points
  if(locationCount <= 20)
   {
    // Fit a low order polynomial

   }
  else
   {
    // Kriging

   }
  
  bufferGeometry(qtree);
}

// =======================================================================================
// Render our part of the scene

vec4 yellowAccentColor = {0.9f, 0.9f, 0.0f, 1.0f};

void LandSurface::draw(Camera& camera)
{
#ifndef USE_TRIANGLE_BUFFER
  VAOs.bind(0);
  VBO->bind();
#endif

  // Highlight where the camera points at
  vec3 pos, dir;
  float lambda;
  camera.copyDirection(pos, dir);
  Quadtree* targetNode = qtree->matchRay(pos, dir, lambda);
  if(targetNode)
    highlightNode(targetNode, yellowAccentColor, 0.3f);
  
  // Draw the main surface
  rect->texture.bind(shader, 0, "earthTexture");
#ifdef USE_TRIANGLE_BUFFER
  tbuf->draw();
#else
  glDrawArrays(GL_TRIANGLES, 0, qtree->landVBOSize);
#endif
  if(targetNode)
    highlightNode(targetNode, yellowAccentColor, 0.0f);
  if(checkGLError(stderr, "LandSurface::draw"))
    exit(-1);
}


// =======================================================================================
