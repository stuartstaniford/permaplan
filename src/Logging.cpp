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

// Logging options to do with parsing and validating OLDF and OTDL files
bool doLogOLDFValidity    = true;    // Log validity problems in the file
bool doLogOLDFDetails     = true;     // Log details of valid OLDF objects
bool doLogOTDLValidity    = true;    // Log validity problems in an OTDL object
bool doLogOTDLDetails     = true;     // Log details of valid OTDL objects

// Logging options to do with interface controls
bool doLogMouseLocation       = true;       // where the mouse is on the screen each frame
bool doLogMouseClick          = true;          // Location of single mouse clicks
bool doLogDoubleClick         = true;         // Location and intra-click timing of double clicks
bool doLogMouseRayPoint       = true;       // Where the mouse is pointing in 3d space
bool doLogLandHeights         = true;         // Log when a new land height is entered.
bool doLogObjectInsertions    = true;    // Log when a new object is inserted in scene.
bool doLogSimulationControls  = true;  // Log control operations on the simulation.

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
