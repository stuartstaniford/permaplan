// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class is the one responsible for drawing the current state of
// the scene at an overall level.

#include "Scene.h"
#include "Shader.h"
#include "HeightMarker.h"
#include "Box.h"
#include "Tree.h"
#include "loadFileToBuf.h"
#include <pthread.h>
#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>

// =======================================================================================
// Constructor, which initializes the geometry

Scene::Scene():
                camera(200.0f, 45.0f),
                sceneObjectTbuf(NULL),
                indicatorTbuf(NULL),
                land(),
                focusObject(NULL),
                lockObject(NULL),
                lighting(),
                simulationSpeed(1.0f), // in years/second
                axes(NULL),
                grid(NULL),
                doSimulation(false),
                simYear(SIMULATION_BASE_YEAR)
#ifdef MULTI_THREADED_SIMULATION
                , simThreads(NULL)                // initialized later in startSimulationThreads
#endif
{
  unsigned minSize = 50;
  // Note that land and qtree have mutual dependencies that means there
  // are several steps in setting them up.
  qtree = new Quadtree(0.0f, 0.0f, (unsigned)(land.rect->width),
                       (unsigned)(land.rect->height),
                       0.0f, 0.0f, 1.0f, 1.0f, minSize, 0u, 0u, NULL);
  land.bufferGeometry(qtree);
  Tree::readTreesFromDesign(qtree);
  pthread_mutex_init(&lock, NULL);
  rebuildVisualObjectBuffer(&sceneObjectTbuf);
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
// C function to launder C++ method into pthread_create

#ifdef MULTI_THREADED_SIMULATION

void* spawnSimThread(void* arg)
{
  int s = (long long)arg;

  LogSimulationControls("Starting simulation thread %d.", s);
  Tree::simulationThreadBase(s);
  return NULL;
}


// =======================================================================================
// Start up the simulation threads

void Scene::startSimulationThreads(void)
{
  const PmodConfig& config = PmodConfig::getConfig();

  simThreads = new pthread_t[config.nSimThreads];
  assert(simThreads);
  
  int pthreadErr;

  for(long long s=0; s<config.nSimThreads; s++)
    if((pthreadErr = pthread_create(simThreads + s, NULL, spawnSimThread, this)) != 0)
      err(-1, "Couldn't spawn simulation thread %lld.\n", s);
}

#endif


// =======================================================================================
// Lock the main scene lock.

void Scene::getLock(void)
{
  pthread_mutex_lock(&lock);
}


// =======================================================================================
// Release the main scene lock.

void Scene::releaseLock(void)
{
  pthread_mutex_unlock(&lock);
}


// =======================================================================================
// Save state from the scene.

void Scene::saveState(void)
{
  const PmodConfig& config = PmodConfig::getConfig();
  if(config.bezWriteFileName)
    qtree->saveSurfaceState(config.bezWriteFileName);
  if(config.writeDesignFileName)
   {
    PmodDesign& theDesign = PmodDesign::getDesign();
    theDesign.writeOLDFFile(land);
   }
}


// =======================================================================================
// Set a rotation matrix to match the currently specified angles

void Scene::startSimulation(void)
{
  doSimulation = true;
}


// =======================================================================================
// Set a rotation matrix to match the currently specified angles

void Scene::pauseSimulation(void)
{
  doSimulation = false;
}


// =======================================================================================
// Set a rotation matrix to match the currently specified angles

void Scene::resetSimulation(void)
{
  simYear = SIMULATION_BASE_YEAR;
  doSimulation = false;
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
  Shader& shader = Shader::getMainShader();
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

void Scene::rebuildVisualObjectBuffer(TriangleBuffer** tbuf)
{
  if(*tbuf)
    delete *tbuf;
  *tbuf = new TriangleBuffer(qtree->vertexTBufSize, qtree->indexTBufSize,
                              (char*)"vObj tbuf");
  qtree->bufferVisualObjects(*tbuf);
  (*tbuf)->sendToGPU(GL_STATIC_DRAW);
}


// =======================================================================================
// The interface has been notified of a new height measurement at the last
// place we double-clicked.

void Scene::newLandHeight(vec3 location)
{
  HeightMarker* H = new HeightMarker(location);
  qtree->storeVisualObject(H);
  if(grid)
    grid->newHeight(location[2]);
  
  rebuildVisualObjectBuffer(&indicatorTbuf);
  //XX also need to rebuild rest of objects once their heights are relative
  
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

void Scene::insertVisibleObject(char* objType, float size, vec3 loc, Material* material)
{
  mat4 transform;
  newObjectTransform(transform, size, loc);
  VisualObject* newObj = getFreshObject(objType, transform);
  
  //XX need to allow the user to edit the object
  LogObjectInsertions("Object inserted: %s (size %.1f) at %.1f, %.1f, %.1f\n",
                      objType, size, loc[0], loc[1], loc[2]);
  qtree->storeVisualObject(newObj);
  rebuildVisualObjectBuffer(&sceneObjectTbuf);
}


// =======================================================================================
// Handle a UI call to insert a new object in the scene (from the insert menu in Window3D)

void Scene::insertTree(Species* species, vec3 loc, float age)
{
  Tree* tree = new Tree(species, loc, age, simYear);
  
  qtree->storeVisualObject(tree);
  rebuildVisualObjectBuffer(&sceneObjectTbuf);
}


// =======================================================================================
// Draw the current state of the scene (called from the main Window3D event loop)

vec4  objColor      = {0.0f, 0.5f, 0.9f, 1.0f};

void Scene::draw(bool mouseMoved, float timeElapsed)
{
  Shader& shader = Shader::getMainShader();
  shader.useProgram();
  setModelMatrix(0.0f, 0.0f);
  lighting.updateGPU();

  // Display the colored axes if configured
  const PmodConfig& config = PmodConfig::getConfig();
  if(config.plotAxes)
   {
    if(!axes)
      axes = new ColoredAxes(1000.0f);  //XX number needs to come from somewhere
    axes->draw();
   }

  // Display grid overlaid over the surface if that is called for
  if(config.plotGrid)
   {
    if(!grid)
      grid = new Grid(land, config.gridSpacing);
    grid->draw();
   }
 
  // Display the main textured land surface
  vec3 L;
  if(land.nextInitialHeightLocation(L))
    newLandHeight(L);
  land.draw(camera);
  
  // Update the trees
  // XX this should be done through quadtree to only simulate currently visible
  // Note this helpful discussion of floating point precision, which will probably
  // crop up as an issue:
  // https://blog.demofox.org/2017/11/21/floating-point-precision/
  if(doSimulation)
   {
    simYear += timeElapsed*simulationSpeed;
#ifndef MULTI_THREADED_SIMULATION
    Tree::growAllTrees(timeElapsed*simulationSpeed);
#endif
    qtree->rebuildTBufSizes();
    rebuildVisualObjectBuffer(&sceneObjectTbuf);
   }
  
#ifdef LOG_TREE_VALIDATION
  qtree->selfValidate(0u);
#endif
  
  // Draw all the objects stored in the quadtree
  if(sceneObjectTbuf)
    //sceneObjectTbuf->draw(Lighted, NULL);
    sceneObjectTbuf->draw(NoTexColor, NULL);
}


// =======================================================================================
