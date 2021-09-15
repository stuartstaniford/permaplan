#include "MainSceneWindow.h"
#include "PmodConfig.h"
#include "PmodDesign.h"
#include "Shader.h"
#include "HttpLoadBalancer.h"
#include "TextureAtlas.h"
#include "Material.h"
#include "Tree.h"
#include "Version.h"
#include "SkySampleModel.h"
#include "RegionList.h"
#include "ResourceManager.h"
#include "GLFWApplication.h"
#include <cstdio>
#include <stdexcept>
#include <pthread.h>


// =======================================================================================
// C function to launder C++ method into pthread_create

void* callProcessConn(void* arg)
{
  HttpLoadBalancer* hd = (HttpLoadBalancer*)arg;
  return hd->processConnections();
}


// =======================================================================================
// Main function

int main (int argc, char* argv[])
{
  // Initialize logging so we can record the rest of the start-up.
  LogInit();
  
  // Process the command line
  PmodConfig config(argc, argv);
  if(!(config.isValid()))
   {
    config.usage();
    return 0;
   }

  initGlobals(config.nSimThreads);
  
  // Initialize everything
  setExternVersions();
  GLFWApplication glfwApp;
  MainSceneWindow window(800, 600);
  glfwApp.addWindow(window);
  Shader shader("src/pmodVert.glsl", "src/pmodFrag.glsl");
  window.camera.makeActive(); // Can't do till shader is set up.
  ResourceManager resources(window);
  PmodDesign design;
  Scene scene;
  SkySampleModel skySampler(design.boundary.referencePoint[0]);
  window.scene = &scene;
  window.imgMenu->scene = &scene;
  TextureAtlas blocksAtlas((char*)"Materials/Blocks");
  TextureAtlas treesAtlas((char*)"Materials/Trees");
  MaterialList materials(blocksAtlas);
  RegionList::loadRoot();
  
  // Start up the debugging http server
  HttpLoadBalancer   httpServer(config.debugPort, scene, *(window.imgMenu));
  int         pthreadErr;
  pthread_t   httpThread;
  if((pthreadErr = pthread_create(&httpThread, NULL, callProcessConn, &httpServer)) != 0)
    err(-1, "Couldn't spawn HTTP server thread in %s.\n", argv[0]);

  // Main display loop
  Window3D::overLoop();
  
  // Orderly shutdown process
  httpServer.shutDownNow = true;
  scene.saveState();
  Window3D::terminate(); // close out the window system
  
  return 0;
}
