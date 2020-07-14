// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// This manages the quadtree used for efficient organization of visual rendering


#ifndef QUADTREE_H
#define QUADTREE_H

#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include <cglm/cglm.h>
#include <algorithm>
#include <list>
#include "VisualObject.h"


// =======================================================================================
// Class variable initialization


class Quadtree
{
 public:
  
  // Instance variables - public
  unsigned    landVBOSize;        //amount of memory required for land vertices,
  unsigned    bufferOffset;       // our offset location in gpu serialized buffer
  unsigned    vertexTBufSize;     //amount of memory required for vertices in triangle buffer,
  unsigned    indexTBufSize;     //amount of memory required for indices in triangle buffer,

  // Member functions - public
  Quadtree(Shader& S, float x, float y, unsigned width, unsigned height, float s, float t,
           float sWidth, float tHeight, unsigned& minSize, unsigned offset);
  ~Quadtree(void);
  void bufferGeometry(VertexBufElement* buf);
  void bufferGeometryLeaf(VertexBufElement* buf);
  void bufferVisualObjects(TriangleBuffer* tbuf);
  void storeVisualObject(VisualObject* obj);
  void redoLandPlanar(vec3 position, vec3 normal);
  void draw(void);
  Quadtree* matchRay(vec3& position, vec3& direction, float& lambda);
  bool diagnosticHTML(HttpDebug* serv, char* path);

 private:
  
  // Instance variables - private
  Shader&                   shader;
  BoundingBox               bbox;
  float                     topLeftZ;
  float                     bottomRightZ;
  vec2                      textureBL;    // bottom left corner of our region in texture s-t space
  vec2                      textureTR;    // top right corner of our region in texture s-t space
  Quadtree*                 kids[4];      // our four child quadtree nodes
  std::list<VisualObject*>  vObjects;     // objects for display that we own
  
  // Member functions - private
  Quadtree* matchChild(vec3& position, vec3& direction, float& lambda);
};

#endif




