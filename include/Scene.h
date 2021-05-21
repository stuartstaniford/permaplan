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
#include "Species.h"
#include "CO2Scenario.h"
#include "TaskQueue.h"
#include "Lockable.h"
#include "InterfaceAction.h"
#include <vector>

#define SIMULATION_BASE_YEAR 1900.0f

// =======================================================================================
// Class variable initialization


class Scene: public Lockable
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
  vec3            focusObjectLocation;
  VisualObject*   focusObject;
  vec3            lockObjectLocation;
  VisualObject*   lockObject;
  VisualObject*   editModeObject;
  vec3            editModeLocation;
  LightingModel   lighting;
  float           simulationSpeed;          //years/sec
  CO2Scenario     rcp8_5;
  std::vector<InterfaceAction*> actions; 
  
  // Member functions - public
  Scene(void);
  ~Scene(void);
  void          draw(bool mouseMoved, float timeElapsed);
  void          loadScenarioFile(void);
  void          startSimulation(void);
  void          pauseSimulation(void);
  void          resetSimulation(void);
  inline float  getSimYear(void) {return simYear;};
  inline bool   simulationActive(void) {return doSimulation;};
  void          findCameraObject(vec3 location);
  float         findCameraHeight(void);
  void          newLandHeight(vec3 location, const char* label);
  void          saveState(void);
  void          updateLightSourcesOnGPU(void);
  VisualObject* getFreshObject(char* objTypeName, mat4 transform);
  void          newObjectTransform(mat4 transform, float initSize, vec3 location);
  void          insertVisibleObject(char* objType, float size, vec3 loc, Material* material);
  void          insertTree(Species* species, vec3 loc, float age);
  void          rebuildVisualObjectBuffer(TriangleBuffer** tbuf);
  void          processEditModeObjectDeselection(void);
  void          processNewEditModeObject(void);
  bool          diagnosticHTMLSimulationSummary(HttpDebug* serv);
  VisualObject* findObjectFromWindowCoords(vec3 location, float clipX, float clipY);
#ifdef MULTI_THREADED_SIMULATION
  void          startSimulationThreads(void);
#endif

 private:
  // Instance variables - private
  ColoredAxes*      axes;
  Grid*             grid;
  bool              doSimulation;
  float             simYear;

  // Member functions - private
  void setModelMatrix(float latt, float longt);
  Scene(const Scene&);                 // Prevent copy-construction
  Scene& operator=(const Scene&);      // Prevent assignment
};

#endif



