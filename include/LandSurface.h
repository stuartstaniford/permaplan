// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// Class which loads a series of records of surface height at various locations
// and produces a tiling of the surface, as well as being able to texture it
// with a provided map.


#ifndef LANDSURFACE_H
#define LANDSURFACE_H

#include <cglm/cglm.h>
#include <vector>
#include "Shader.h"
#include "PmodDesign.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "TexturedRect.h"
#include "Quadtree.h"
#include "Camera.h"
#include "HeightMarker.h"
#include "BezierPatch.h"


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
  void bufferGeometry(Quadtree* q);
  ~LandSurface(void);
  void newLandHeight(HeightMarker* hM);
  void redoBezierLandSurface(BezierPatch* bez);
  void draw(Camera& camera);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  Shader&               shader;
  PmodDesign&           design;
  TriangleBuffer*       tbuf;
  unsigned              locationCount;
  std::vector<float*>   heightLocations;
  bool                  inFitMode;
#ifdef VISUALIZE_FITTING
  TriangleBuffer*       fitTBuf;
#endif

  //Texture               texture;
  
  // Member functions - private
  void highlightNode(Quadtree* targetNode, vec4& color, float accent);


};

#endif



