// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// This is where the implementation of the logging system goes (most of it is macros
// in Logging.h)

// =======================================================================================

#include <err.h>
#include <stdio.h>
#include "Logging.h"
#include "Vertex.h"

FILE* LogFile;

void LogInit(void)
{
  char* fileName = (char*)"pmod.log";
  
  LogFile = fopen(fileName, "w");
  if(!LogFile)
    err(-1, "Couldn't create log file %s\n", fileName);
  
  LogStructureSizes("Size of Vertex: %lu bytes.\n", sizeof(Vertex));
  
}
