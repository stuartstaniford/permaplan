// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class is the one responsible for drawing the current state of
// the scene at an overall level.

#include "Scene.h"
#include "SkySampleModel.h"
#include "ControlGroup.h"
#include "Shader.h"
#include "HeightMarker.h"
#include "Box.h"
#include "Tree.h"
#include "Building.h"
#include "loadFileToBuf.h"
#include "HttpDebug.h"
#include <pthread.h>
#include <stdlib.h>
#include <err.h>
#include <GL/glew.h>


// =======================================================================================
/// @brief Constructor, which initializes the setup of the scene.
///
///This does things like setting up the quadtree, setting up the landsurface, and reading
///in visiable things like trees, buildings, etc from the OLDF file and putting them into
///the quadtree.

Scene::Scene():
                camera(200.0f, 45.0f),
                sceneObjectTbuf(NULL),
                indicatorTbuf(NULL),
                land(),
                focusObject(NULL),
                lockObject(NULL),
                editModeObject(NULL),
                lighting(),
                simulationSpeed(1.0f), // in years/second
                rcp8_5((char*)"RCP8.5"),
                axes(NULL),
                grid(NULL),
                doSimulation(false),
                simYear(SIMULATION_BASE_YEAR)
{
  unsigned minSize = 50;
  // Note that land and qtree have mutual dependencies that means there
  // are several steps in setting them up.
  qtree = new Quadtree(land.rectOffset[0], land.rectOffset[1], (unsigned)(land.rect->width),
                       (unsigned)(land.rect->height),
                       0.0f, 0.0f, 1.0f, 1.0f, minSize, 0u, 0u, NULL);
  land.bufferGeometry(qtree);
  Tree::readTreesFromDesign(qtree);
  Building::readBuildingsFromDesign(qtree, *this);
  rebuildVisualObjectBuffer(&sceneObjectTbuf);
}


// =======================================================================================
/// @brief Destructor

Scene::~Scene(void)
{
  if(axes)
    delete axes;  
  saveState();
}


// =======================================================================================
/// @brief Save state from the scene if required by configuration variables.

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
/// @brief API to turn on simulation.
/// 
/// The simulating will actually occur in future calls to Scene::draw, but this sets up
/// relavant state for it to happen.

void Scene::startSimulation(void)
{
  doSimulation = true;
}


// =======================================================================================
/// @brief API to stop simulating for the time being.

void Scene::pauseSimulation(void)
{
  doSimulation = false;
}


// =======================================================================================
/// @brief API to reset the simulation to the beginning.
/// @todo This function is incomplete and the operation doesn't really work.  All the 
/// visual objects in the scene need to be re-initialized.

void Scene::resetSimulation(void)
{
  simYear = SIMULATION_BASE_YEAR;
  doSimulation = false;
}


// =======================================================================================
/// @brief Set the model matric in the Shader based on two rotation angles.
/// @param latt The "latitude" of the point at which to see overhead.
/// @param longt The "longtitude" of the point at which to see overhead.
/// @todo The model matrix is barely used in permaplan at present.  Making more use of it
/// would probably require breaking up the TriangleBuffers so that we could switch 
/// matrices between rendering each buffer.

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

void Scene::rebuildVisualObjectBuffer(TriangleBuffer** tbuf, bool dumpBuf)
{
#ifdef LOG_TRIANGLE_BUF_REBUILDS
unsigned oldVCount = 0u;
unsigned oldICount = 0u;
#endif

  if(*tbuf)
   {
#ifdef LOG_TRIANGLE_BUF_REBUILDS
    oldVCount = (*tbuf)->vCount;
    oldICount = (*tbuf)->iCount;
#endif
    delete *tbuf;
   }
  
  *tbuf = new TriangleBuffer(qtree->vertexTBufSize, qtree->indexTBufSize,
                              (char*)"vObj tbuf");
  LogTriangleBufRebuilds("TriangleBuffer rebuild of %s: %u,%u to %u,%u.\n", 
                          (*tbuf)->bufName, oldVCount, oldICount, (*tbuf)->vCount, (*tbuf)->iCount);
  qtree->bufferVisualObjects(*tbuf);
  if(dumpBuf)
    (*tbuf)->dumpBuffer();
  (*tbuf)->sendToGPU(GL_STATIC_DRAW);
}


// =======================================================================================
// The interface has been notified of a new height measurement at the last
// place we double-clicked.

void Scene::newLandHeight(vec3 location, const char* label)
{
  HeightMarker* H = new HeightMarker(location);
  if(label)
    H->setLabel(label);
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
// Obtain the right kind of object based on the insertion menu text.

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
// This is called when an object has just been double-clicked on, and now we need to set
// up controls to allow it to be moved around, rescaled, etc.

void Scene::processEditModeObjectDeselection(void)
{

}


// =======================================================================================
// This is called when an object has just been double-clicked on, and now we need to set
// up controls to allow it to be moved around, rescaled, etc.

void Scene::processNewEditModeObject(void)
{
  ControlGroup* controlGroup = new ControlGroup(editModeObject);
  editModeObject->removeFromQuadtree();
  qtree->storeVisualObject(controlGroup);
  rebuildVisualObjectBuffer(&sceneObjectTbuf);
}


// =======================================================================================
// Handle a UI call to insert a new object in the scene (called from various MenuPanels
// which construct the respective kind of objects)

void Scene::insertVisualObject(VisualObject* obj)
{  
  //XX need to allow the user to edit the object
#ifdef LOG_OBJECT_INSERTIONS
  float x,y;
  obj->getGroundContact(x,y);
  LogObjectInsertions("Object inserted in scene: %s at %.1f, %.1f\n", obj->objectName(), x, y);
#endif
  qtree->storeVisualObject(obj);
#ifdef LOG_DUMP_OBJECT_BUFFER
  rebuildVisualObjectBuffer(&sceneObjectTbuf, true);
#else
  rebuildVisualObjectBuffer(&sceneObjectTbuf);
#endif
}


// =======================================================================================
// Legacy: Handle a UI call to insert a new object in the scene (from the insert menu 
// in Window3D)

void Scene::insertVisibleObject(char* objType, float size, vec3 loc, Material* material)
{
  mat4 transform;
  newObjectTransform(transform, size, loc);
  VisualObject* newObj = getFreshObject(objType, transform);
  insertVisualObject(newObj);
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
// Provide a summary of the simulation status (within an HTML page)

bool Scene::diagnosticHTMLSimulationSummary(HttpDebug* serv)
{
  float year = getSimYear();
  float CO2 = rcp8_5.getConcentration(year);
  httPrintf("<hr><center><span name=\"simSummary\">\n");

  if(simulationActive())
   {
    httPrintf("<b>Simulation Speed:</b> %.2f.\n", simulationSpeed);
   }
  else
    httPrintf("<b>Simulation:</b> Paused.\n");
  httPrintf("<b>Year:</b> %.0f.\n", year);
  httPrintf("<b>CO2:</b> %.0fppm.\n", CO2);
  
  httPrintf("</span></center>\n");
  return true;
}


// =======================================================================================
// Draw the current state of the scene (called from the main Window3D event loop)

vec4  objColor      = {0.0f, 0.5f, 0.9f, 1.0f};

void Scene::draw(bool mouseMoved, float timeElapsed)
{
  PmodDesign& design = PmodDesign::getDesign();
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
  unless(design.designBoxValid)
   {
    vec3 L;
    const char* label;
    if(land.nextInitialHeightLocation(L, label))
      newLandHeight(L, label);
    else
     {
      design.boundary.extendBoundingBox(design.designBox);
      design.addBoxTolerances();
      design.designBoxValid = true;
     }
   } 
  land.draw(camera);
  
  // Update the trees
  // XX this should be done through quadtree to only simulate currently visible
  // Note this helpful discussion of floating point precision, which will probably
  // crop up as an issue:
  // https://blog.demofox.org/2017/11/21/floating-point-precision/
  if(doSimulation)
   {
    simYear += timeElapsed*simulationSpeed;
    SkySampleModel::getSkySampleModel().updateIfNeeded(simYear);
#ifdef MULTI_THREADED_SIMULATION
    Tree::analyzeTreeGraph(timeElapsed*simulationSpeed, *this);
#else
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
