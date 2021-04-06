// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// A place for macros that we want to be able to use in many locations and also to define
// global variables used throughout the program.


#ifndef GLOBAL_MACRO_DEFNS_H
#define GLOBAL_MACRO_DEFNS_H

// =======================================================================================
// Global macro definitions

#define unless(X) if(!(X))
#define until(X) while(!(X))
#define EPSILON 0.000000001f
#define isWhiteSpace(x) ( (x)==' ' || (x)=='\t' || (x)=='\n' || (x)=='\r' || (x)=='\f' )

#define MULTI_THREADED_SIMULATION

// =======================================================================================
// Function prototypes

void initGlobals(int nThreads);

// =======================================================================================
// Extern definitions of global variables here
#ifndef GLOBAL_IMPLEMENTATION

class TaskQueueFarm;
extern TaskQueueFarm* threadFarm;

#endif

#endif


