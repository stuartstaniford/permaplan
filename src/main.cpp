#include <cstdio>
#include <stdexcept>
#include <pthread.h>
#include "Window3D.h"
#include "PmodConfig.h"
#include "PmodDesign.h"
#include "Shader.h"
#include "HttpDebug.h"
#include "TextureAtlas.h"
#include "Material.h"
#include "Tree.h"

// =======================================================================================
// C function to launder C++ method into pthread_create

void* callProcessConn(void* arg)
{
  HttpDebug* hd = (HttpDebug*)arg;
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

  // Initialize everything
  Window3D window(800, 600);
  Shader shader("src/pmodVert.glsl", "src/pmodFrag.glsl");
  PmodDesign design;
  Tree::readTreesFromDesign();
  //checkGLError(stderr, "after Tree\n");
  Scene scene;
  window.scene = &scene;
  window.imgMenu->scene = &scene;
  window.scriptController = new ScriptController();
  TextureAtlas blocksAtlas((char*)"Materials/Blocks");
  TextureAtlas treesAtlas((char*)"Materials/Trees");
  MaterialList materials(blocksAtlas);
  
  // Start up the debugging http server
  HttpDebug   httpServer(config.debugPort, scene);
  int         pthreadErr;
  pthread_t   httpThread;
  //pthread_mutex_init(&glLock, NULL);
  if((pthreadErr = pthread_create(&httpThread, NULL, callProcessConn, &httpServer)) != 0)
    err(-1, "Couldn't spawn HTTP server thread in %s.\n", argv[0]);
  
  // Main simulation loop
  window.loop(httpServer);

 return 0;
}
