// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// Class which loads a series of records of surface height at various locations
// and produces a tiling of the surface, as well as being able to texture it
// with a provided map.


#ifndef LANDSURFACE_H
#define LANDSURFACE_H

#include <cglm/cglm.h>
#include "Shader.h"
#include "PmodDesign.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "TexturedRect.h"
#include "Quadtree.h"
#include "Camera.h"
#include "HeightMarker.h"


// =======================================================================================
// Class variable initialization

class LandSurface
{
 public:
  
  // Instance variables - public
  TexturedRect*   rect;
  Quadtree*       qtree;
  
  // Member functions - public
  LandSurface(Shader& S, PmodDesign& D);
  void setUpVBO(Quadtree* q);
  ~LandSurface(void);
  void newLandHeight(HeightMarker* hM);
  void draw(Camera& camera);

 private:
  
  // Instance variables - private
  Shader&               shader;
  PmodDesign&           design;
  VertexArrayObject     VAOs;
  VertexBufferObject*   VBO;
  unsigned              locationCount;

  //Texture               texture;
  
  // Member functions - private
  void highlightNode(Quadtree* targetNode, vec4& color, float accent);


};

#endif



