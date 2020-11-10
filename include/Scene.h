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
#include "Material.h"
#include "LightingModel.h"


// =======================================================================================
// Class variable initialization


class Scene
{
 public:
  
  // Instance variables - public
  Camera          camera;
  Quadtree*       qtree;
  TriangleBuffer* sceneObjectTbuf;
  TriangleBuffer* indicatorTbuf;
  LandSurface     land;
  mat4            model;
  vec3            lastMouseLocation;
  vec3            lastMouseDirection;
  vec3            lastDoubleClick;
  LightingModel   lighting;
  
  // Member functions - public
  Scene(void);
  ~Scene(void);
  void          draw(bool mouseMoved);
  void          findCameraObject(vec3 location);
  float         findCameraHeight(void);
  void          newLandHeight(vec3 location);
  void          saveState(void);
  void          updateLightSourcesOnGPU(void);
  VisualObject* getFreshObject(char* objTypeName, mat4 transform);
  void          newObjectTransform(mat4 transform, float initSize, vec3 location);
  void          insertVisibleObject(char* objType, float size, vec3 loc, Material* material);
  void          rebuildVisualObjectBuffer(TriangleBuffer** tbuf);
  VisualObject* findObjectFromWindowCoords(vec3 location, float clipX, float clipY);

 private:
  // Instance variables - private
  ColoredAxes*      axes;
  Grid*             grid;

  // Member functions - private
  void setModelMatrix(float latt, float longt);
};

#endif



