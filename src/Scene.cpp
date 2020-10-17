// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class is the one responsible for drawing the current state of
// the scene at an overall level.

#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>
#include "Scene.h"
#include "Shader.h"
#include "HeightMarker.h"
#include "Box.h"

// =======================================================================================
// Constructor, which initializes the geometry

Scene::Scene(Shader& S):
                shader(S),
                camera(shader, 200.0f, 45.0f),
                tbuf(NULL),
                land(shader),
                axes(NULL),
                grid(NULL)
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
  saveState();
}


// =======================================================================================
// Save state from the scene.

void Scene::saveState(void)
{
  const PmodConfig& config = PmodConfig::getConfig();
  if(config.bezWriteFileName)
    qtree->saveSurfaceState(config.bezWriteFileName);
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
// Find the object at a given screen location (eg the mouse position)

VisualObject* Scene::findObjectFromWindowCoords(vec3 location, float clipX, float clipY)
{  
  camera.rayFromScreenLocation(lastMouseLocation, lastMouseDirection, clipX, clipY);

  // Now find what we point to
  float lambda;
  VisualObject* obj = qtree->matchRay(lastMouseLocation, lastMouseDirection, lambda);
  if(obj)
   {
    glm_vec3_scale(lastMouseDirection, lambda, lastMouseDirection);
    glm_vec3_add(lastMouseLocation, lastMouseDirection, location);
    LogMouseRayPoint("Mouse on object %s at %.1f, %.1f, %.1f\n",
                     obj->objectName(), location[0], location[1], location[2]);
    return obj;
   }
  LogMouseRayPoint("No object under mouse.\n");
  return NULL;
}


// =======================================================================================
//XXX Temporary hack - toss the old buffer and make a new one

void Scene::rebuildVisualObjectBuffer(void)
{
  if(tbuf)
    delete tbuf;
  tbuf = new TriangleBuffer(qtree->vertexTBufSize, qtree->indexTBufSize);
  qtree->bufferVisualObjects(tbuf);
  tbuf->sendToGPU(GL_STATIC_DRAW);
}


// =======================================================================================
// The interface has been notified of a new height measurement at the last
// place we double-clicked.

void Scene::newLandHeight(vec3 location)
{
  HeightMarker* H = new HeightMarker(location);
  qtree->storeVisualObject(H);
  grid->newHeight(location[2]);
  
  rebuildVisualObjectBuffer();
  
  //Redo the landsurface here, in light of the new height observation
  land.newLandHeight(H);
  if(land.getLocationCount() == 1)
    camera.teleportUp(location[2]);
}


// =======================================================================================
// Draw the current state of the scene (called from the main Window3D event loop)

VisualObject* Scene::getFreshObject(char* objTypeName, mat4 transform)
{
  VisualObject* returnVal = NULL;
  
  if(strcmp(objTypeName, "Block") == 0)
    returnVal = (VisualObject*)new Box(transform);

  return returnVal;
}


// =======================================================================================
// Create the transformation matrix which will allow the new object to be visible in the
// correct initial location.

//XX likely this should be moved into visualObject and be done in a way that avoids
// copying the matrix in the object state.

void Scene::newObjectTransform(mat4 transform, float initSize, vec3 location)
{
  // Remember that cglm does right multiplication, so the transformations are applied
  // to the incoming object in reverse order to their order in the code.
  glm_mat4_identity(transform);
  glm_translate(transform, location);
  glm_scale_uni(transform, initSize);
}


// =======================================================================================
// Handle a UI call to insert a new object in the scene (from the insert menu in Window3D)

void Scene::insertVisibleObject(char* objTypeName, float initSize, vec3 location)
{
  mat4 transform;
  newObjectTransform(transform, initSize, location);
  VisualObject* newObj = getFreshObject(objTypeName, transform);
  
  //XX need to allow the user to edit the object
  LogObjectInsertions("Object inserted: %s (size %.1f) at %.1f, %.1f, %.1f\n",
                      objTypeName, initSize, location[0], location[1], location[2]);
  qtree->storeVisualObject(newObj);
  rebuildVisualObjectBuffer();
}


// =======================================================================================
// Draw the current state of the scene (called from the main Window3D event loop)

vec4  objColor      = {0.0f, 0.5f, 0.9f, 1.0f};

void Scene::draw(bool mouseMoved)
{
  shader.useProgram();
  setModelMatrix(0.0f, 0.0f);

  // Display the colored axes if configured
  const PmodConfig& config = PmodConfig::getConfig();
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
  vec3 L;
  if(land.nextInitialHeightLocation(L))
    newLandHeight(L);
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


// =======================================================================================

