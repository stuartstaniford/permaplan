// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class is the one responsible for drawing the current state of
// the scene at an overall level.

#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>
#include "Scene.h"
#include "Shader.h"
#include "HeightMarker.h"

// =======================================================================================
// Constructor, which initializes the geometry

Scene::Scene(Shader& S, PmodDesign& des, PmodConfig& conf):
                shader(S),
                camera(shader, 200.0f, 45.0f),
                design(des),
                tbuf(NULL),
                land(shader, design),
                axes(NULL),
                grid(NULL),
                config(conf)
{
  unsigned minSize = 10;
  // Note that land and qtree have mutual dependencies that means there
  // are several steps in setting them up.
  qtree = new Quadtree(shader, 0.0f, 0.0f, (unsigned)(land.rect->width),
                       (unsigned)(land.rect->height),
                       0.0f, 0.0f, 1.0f, 1.0f, minSize, 0u);
  land.bufferGeometry(qtree);
}


// =======================================================================================
// Destructor

Scene::~Scene(void)
{
  if(axes)
    delete axes;
}


// =======================================================================================
// Set a rotation matrix to match the currently specified angles

void Scene::setModelMatrix(float latt, float longt)
{
  static vec3 zAxis = {0.0f, 0.0f, 1.0f};
  static vec3 yAxis = {0.0f, 1.0f, 0.0f};
  
  glm_mat4_identity(model);
  glm_rotate(model, latt/180.0f*M_PI, yAxis);
  glm_rotate(model, longt/180.0f*M_PI, zAxis);
  shader.setUniform("model", model);
  if(checkGLError(stderr, "Scene::setModelMatrix"))
    exit(-1);
}


// =======================================================================================
// Find the object at camera focus
// (Presently returning void until object interface defined)

void Scene::findCameraObject(vec3 location)
{
  vec3 pos, dir;
  float lambda;
  camera.copyDirection(pos, dir);
  /*Quadtree* targetNode =*/ qtree->matchRay(pos, dir, lambda);
  glm_vec3_scale(dir, lambda, dir);
  glm_vec3_add(pos, dir, location);
}


// =======================================================================================
// How high is the camera above the scene?

float Scene::findCameraHeight(void)
{
  vec3 pos, dir;
  camera.copyDirection(pos, dir);
  
  //XXX need to compute land height below camera
  
  return pos[2];
}


// =======================================================================================
// The interface has been notified of a new height measurement at the camera focus.

void Scene::newLandHeight(float& z)
{
  vec3 newLocation;
  findCameraObject(newLocation);
  newLocation[2] = z;
  HeightMarker* H = new HeightMarker(newLocation);
  qtree->storeVisualObject(H);
  
  // XXX Temporary hack - toss the old buffer and make a new one
  if(tbuf)
    delete tbuf;
  tbuf = new TriangleBuffer(qtree->vertexTBufSize, qtree->indexTBufSize);
  qtree->bufferVisualObjects(tbuf);
  tbuf->sendToGPU(GL_STATIC_DRAW);
  
  //Redo the landsurface here, in light of the new height observation
  land.newLandHeight(H);
}


// =======================================================================================
// Draw the current state of the scene (called from the main Window3D event loop)

vec4  objColor  = {0.0f, 0.5f, 0.9f, 1.0f};

int count = 1;
void Scene::draw(void)
{
  shader.useProgram();
  setModelMatrix(0.0f, 0.0f);

  // Display the colored axes if configured
  if(config.plotAxes)
   {
    if(!axes)
      axes = new ColoredAxes(shader, 1000.0f);  //XX number needs to come from somewhere
    axes->draw();
   }

  // Display grid overlaid over the surface if that is called for
  if(config.plotGrid)
   {
    if(!grid)
      grid = new Grid(shader, land, config.gridSpacing);
    grid->draw();
   }

  // Display the main textured land surface
  land.draw(camera);
  
  // Draw all the objects stored in the quadtree
  if(tbuf)
   {
    shader.setUniform("fixedColor", true);
    shader.setUniform("theColor", objColor);
    tbuf->draw();
    shader.setUniform("fixedColor", false);
   }
}
