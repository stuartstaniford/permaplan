// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class is the one responsible for drawing the current state of
// the scene at an overall level.


#ifndef SCENE_H
#define SCENE_H

#include "Shader.h"
#include "ColoredAxes.h"
#include "PmodConfig.h"
#include "LandSurface.h"
#include "Camera.h"
#include "PmodDesign.h"
#include "Grid.h"
#include "Quadtree.h"
#include "TriangleBuffer.h"


// =======================================================================================
// Class variable initialization


class Scene
{
 public:
  
  // Instance variables - public
  Shader&         shader;
  Camera          camera;
  PmodDesign&     design;
  Quadtree*       qtree;
  TriangleBuffer* tbuf;
  LandSurface     land;
  mat4            model;

  // Member functions - public
  Scene(Shader& S, PmodDesign& des, PmodConfig& C);
  ~Scene(void);
  void  draw(void);
  void  findCameraObject(vec3 location);
  float findCameraHeight(void);
  void  newLandHeight(float& z);

 private:
  // Instance variables - private
  ColoredAxes*      axes;
  Grid*             grid;
  PmodConfig&       config;

  // Member functions - private
  void setModelMatrix(float latt, float longt);
};

#endif



