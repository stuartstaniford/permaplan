// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// This is where the implementation of the logging system goes (most of it is macros
// in Logging.h)

// =======================================================================================

#define LOGGING_IMPLEMENTATION

#include <err.h>
#include <stdio.h>
#include "Logging.h"
#include "Vertex.h"


// =======================================================================================
// Variables that control real time logging when compiled in

// Logging options to do with overall control flow and speed
bool doLogFrameStarts     = true; // Log each frame as it begins
bool doLogCloseDown       = true; // Log when we exit
bool doLogStructureSizes  = true; // Log the size of structures on this platform at startup.
bool doLogOpenGLConstants = true; // Log various openGL parameters


// =======================================================================================
// Function to start up logging

FILE* LogFile;

void LogInit(void)
{
  char* fileName = (char*)"permaplan.log";
  
  LogFile = fopen(fileName, "w");
  if(!LogFile)
    err(-1, "Couldn't create log file %s\n", fileName);
  
  LogStructureSizes("Size of Vertex: %lu bytes.\n", sizeof(Vertex));
  
}
