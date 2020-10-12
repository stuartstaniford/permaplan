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
  Quadtree*       qtree;
  TriangleBuffer* tbuf;
  LandSurface     land;
  mat4            model;
  vec3            lastMouseLocation;
  vec3            lastMouseDirection;
  vec3            lastDoubleClick;

  // Member functions - public
  Scene(Shader& S);
  ~Scene(void);
  void          draw(bool mouseMoved);
  void          findCameraObject(vec3 location);
  float         findCameraHeight(void);
  void          newLandHeight(vec3 location);
  void          saveState(void);
  VisualObject* findObjectFromWindowCoords(vec3 location, float clipX, float clipY);

 private:
  // Instance variables - private
  ColoredAxes*      axes;
  Grid*             grid;

  // Member functions - private
  void setModelMatrix(float latt, float longt);
};

#endif



