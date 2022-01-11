// Main program for the permaserv HTTP server which exists to run at central HQ and
// supply permaplan instances with values required from very large databases that it
// wouldn't make sense to download in their entirety (eg solar DIF and DNI values,
// values from soil databases at particular locations, temperature or precipitation
// time series from climate model runs).

#include "HttpLoadBalancer.h"
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
    
  // Start up the debugging http server
  HttpLoadBalancer   httpServer(config.debugPort, scene, glfwApp);
  int         pthreadErr;
  pthread_t   httpThread;
  if((pthreadErr = pthread_create(&httpThread, NULL, callProcessConn, &httpServer)) != 0)
    err(-1, "Couldn't spawn HTTP server thread in %s.\n", argv[0]);
  
  sleep(1e8);
  
  return 0;
}
