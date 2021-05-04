// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// A place for macros that we want to be able to use in many locations and also to define
// global variables used throughout the program.

#define GLOBAL_IMPLEMENTATION

#include "Global.h"
#include "TaskQueueFarm.h"
#include <stdio.h>

// =======================================================================================
// Global variables

TaskQueueFarm* threadFarm = NULL;

// =======================================================================================
// Initialization of the global variables defined in here.

void initGlobals(int nThreads)
{
#ifdef MULTI_THREADED_SIMULATION
  threadFarm = new TaskQueueFarm(nThreads);  
#endif
  
}


// =======================================================================================
// Extract a ':' separated vec3 from a (URL) string.

bool extractColonVecN(char* path, int N, float* dest)
{
  char* next;
  for(int i=0; i<N; i++)
   {
    next = index(path, ':');
    unless(next)
      return false;
    *next = '\0';
    dest[i] = atof(path);
    path = next + 1;
   }
  return true;
}


// =======================================================================================
