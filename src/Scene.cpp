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
#include "Window3D.h"
#include "loadFileToBuf.h"
#include "HttpDebug.h"
#include <pthread.h>
#include <stdlib.h>
#include <err.h>


// =======================================================================================
/// @brief Constructor, which initializes the setup of the scene.
///
///This does things like setting up the quadtree, setting up the landsurface, and reading
///in visible things like trees, buildings, etc from the OLDF file and putting them into
///the quadtree.

Scene::Scene():
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
/// @brief Set up the OpenGL context associated with a new window that has been opened
/// with shared state from an existing window.  This is in Scene because it requires
/// doing things across multiple aspects of the Scene, and that is co-ordinated from here.
///
/// See [this example GLFW progam)[https://github.com/glfw/glfw/blob/master/examples/sharing.c]
/// for some ideas about the issues this function is handling.

void Scene::configureOpenGLForNewWindow(void)
{
  
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
/// @brief Find the object at camera focus
/// @returns A pointer to the VisualObject found, NULL if nothing.
/// @param location A vec3 where the location of the camera intersection will be stored.
/// @param camera A reference to the particular camera who'se focus we are interested in
/// (since with multiple windows there can be multiple cameras in play).

VisualObject* Scene::findCameraObject(vec3 location, Camera& camera)
{
  vec3 pos, dir;
  float lambda;
  camera.copyDirection(pos, dir);
  VisualObject* targetNode = qtree->matchRay(pos, dir, lambda);
  if(targetNode)
   {
    glm_vec3_scale(dir, lambda, dir);
    glm_vec3_add(pos, dir, location);
   }
  return targetNode;
}


// =======================================================================================
/// @brief How high is the camera above the scene?
/// @returns A floating point value for the height of the camera.  Currently is the 
/// absolute height.

float Scene::findCameraHeight(Camera& camera)
{
  vec3 pos, dir;
  camera.copyDirection(pos, dir);
  
  //XXX need to compute land height below camera
  
  return pos[2];
}


// =======================================================================================
/// @brief Find the object at a given screen location (eg the mouse position).
/// @returns A pointer to the VisualObject found, or NULL if none found.
/// @param location A vec3 to store the location at which we intersected the object
/// @param clipX The window screen X coordinate requested.
/// @param clipY The window screen Y coordinate requested.

VisualObject* Scene::findObjectFromWindowCoords(Camera& camera, vec3 location, 
                                                                  float clipX, float clipY)
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
/// @brief Rebuild the visual object buffer and send to the GPU.
/// @param tbuf A pointer to the pointer to the TriangleBuffer.  
/// @param dumpBuf If true, the triangle buffer will be dumped to a file for debugging.
/// @todo This is really a temporary hack - it tosses the old buffer and makes a new one.

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
/// @brief Process notification of a new altitude measurement.
/// 
/// The interface has been notified of a new height measurement (eg at the last place 
/// we double-clicked).
/// @param location A vec3 of the new height location (x,y) position in spaceUnits
/// relative to the referencePoint and absolute altitude in the z-coordinate.
/// @param label A C-string allowing for a human annotation describing the location of 
/// this particular observation.
/// @todo also need to rebuild rest of visual objects once their heights are relative

void Scene::newLandHeight(vec3 location, const char* label)
{
  HeightMarker* H = new HeightMarker(location);
  if(label)
    H->setLabel(label);
  qtree->storeVisualObject(H);
  if(grid)
    grid->newHeight(location[2]);
  
  rebuildVisualObjectBuffer(&indicatorTbuf);
  
  //Redo the landsurface here, in light of the new height observation
  land.newLandHeight(H);
  if(land.getLocationCount() == 1)
   {
    Window3D& win = Window3D::getMainWin();
    win.camera.teleportUp(location[2]);
   }
  
  //XX Redo the visual objects
}


// =======================================================================================
/// @brief Handle a UI operation to insert a new object in the scene. 
/// 
/// This is called from various MenuPanels which construct the respective kind of 
/// objects, and is the correct current way of doing this.  We handle appropriate logging
/// insert the object in the quadtree, and then rebuild the visual object buffers.
/// @param obj Pointer to the new VisualObject to be inserted.

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
/// @brief Obtain the right kind of object based on the insertion menu text.
/// @todo Only used for inserting blocks, scheduled for demolition.

VisualObject* Scene::getFreshObject(char* objTypeName, mat4 transform)
{
  VisualObject* returnVal = NULL;
  
  if(strcmp(objTypeName, "Block") == 0)
    returnVal = (VisualObject*)new Box(transform);
  
  return returnVal;
}


// =======================================================================================
/// Create the transformation matrix which will allow the new object to be visible in the
/// correct initial location.
/// @todo Only used for inserting blocks, scheduled for demolition.

void Scene::newObjectTransform(mat4 transform, float initSize, vec3 location)
{
  // Remember that cglm does right multiplication, so the transformations are applied
  // to the incoming object in reverse order to their order in the code.
  glm_mat4_identity(transform);
  glm_translate(transform, location);
  glm_scale_uni(transform, initSize);
}


// =======================================================================================
/// @brief Handle a UI call to insert a new object in the scene (from the insert menu 
/// in Window3D)
/// @todo Only used for inserting blocks, scheduled for demolition.

void Scene::insertVisibleObject(char* objType, float size, vec3 loc, Material* material)
{
  mat4 transform;
  newObjectTransform(transform, size, loc);
  VisualObject* newObj = getFreshObject(objType, transform);
  insertVisualObject(newObj);
}


// =======================================================================================
/// @brief This is called when an object has been deselected and we need to remove the
/// control objects around it.
/// @todo This is only a stub right now.

void Scene::processEditModeObjectDeselection(void)
{

}


// =======================================================================================
/// @brief Called when an object has been double-clicked on, and now we need to set up 
/// controls to allow it to be moved around, rescaled, etc.

void Scene::processNewEditModeObject(void)
{
  ControlGroup* controlGroup = new ControlGroup(editModeObject);
  editModeObject->removeFromQuadtree();
  qtree->storeVisualObject(controlGroup);
  rebuildVisualObjectBuffer(&sceneObjectTbuf);
}


// =======================================================================================
/// @brief Provide a summary of the simulation status (within an HTML page)

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
/// @brief Draw the current state of the scene.
/// 
/// This is called from a main Window3D event loop and coordinates all drawing of the
/// scene.
/// @param camera A reference to the Camera object of thw window that wishes to render us,
/// so that the scene can be shown from the correct angle/perspective.
/// @param timeElapsed The amount of time elapsed in seconds since the last call (for
/// use in when the simulation is running and needs to be updated).

void Scene::draw(Camera& camera, float timeElapsed)
{
  PmodDesign& design = PmodDesign::getDesign();
  //Shader& shader = Shader::getMainShader();
  //shader.useProgram();
  checkGLError(stderr, "Temp\n");
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
  if(checkGLError(stderr, "End of Scene::draw"))
    exit(-1);
}


// =======================================================================================
