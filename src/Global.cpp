// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// A place for macros that we want to be able to use in many locations and also to define
// global variables used throughout the program.

#define GLOBAL_IMPLEMENTATION

#include "Global.h"
#include "TaskQueueFarm.h"
#include "Logging.h"
#include <stdio.h>


// =======================================================================================
// Global variables

TaskQueueFarm* threadFarm = NULL;

// =======================================================================================
// Initialization of the global variables defined in here.

void initGlobals(int nThreads)
{
#ifdef MULTI_THREADED_SIMULATION
  threadFarm = new TaskQueueFarm(nThreads, (char*)"treeSimFarm");  
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
     {
      LogRequestErrors("extractColonVecN failed with i=%d from 0..%d and path=%s.\n", 
                                                                            i, N-1, path);
      return false;
     }
    *next = '\0';
    dest[i] = atof(path);
    path = next + 1;
   }
  return true;
}


// =======================================================================================
// Determine if a float is a valid number.  Returns NULL if it's fine, 
// otherwise a string about the problem.

char* errorInFloat(float f)
{  
  if(isnan(f))
    return (char*)"Field is not a valid number.";
  
  if(isinf(f))
    return (char*)"Field value is infinite.";
  
  return NULL;
}


// =======================================================================================
// Determine if a character string is a valid number.  Returns NULL if it's fine, 
// otherwise a string about the problem.

char* errorInNumericCharacterField(char* field)
{
  if(strlen(field) == 0)
    return (char*)"No data in field.";
  
  char* endPtr;
  float f = strtof(field, &endPtr);
  unless(endPtr)
    return (char*)"Field is not valid numerical data.";
  
  if(isnan(f))
    return (char*)"Field is not a valid number.";
  
  if(isinf(f))
    return (char*)"Field value is infinite.";
  
  return NULL;
}


// =======================================================================================
// Determine the extension type of a filename.

FileExtension findExtension(const char* fileName)
{
  char* extension = rindex(fileName, '.');
  unsigned len    = strlen(fileName);
  if(!extension)
    return ExtNoExtension;
  extension++;
  if(extension - fileName >= len)
    return ExtEndsInDot; 
 
  //XX should probably set up an unordered_map here to make this scale better.
  
  // Definitely image types
  if(strcmp(extension, "jpg")==0 || strcmp(extension, "JPG")==0
      || strcmp(extension, "jpeg")==0 || strcmp(extension, "JPEG")==0)
    return ExtJpeg;
  if(strcmp(extension, "png")==0 || strcmp(extension, "PNG")==0)
    return ExtPng;
  if(strcmp(extension, "gif")==0 || strcmp(extension, "GIF")==0)
    return ExtGif;
  if(strcmp(extension, "bmp")==0 || strcmp(extension, "BMP")==0)
    return ExtBmp;

  // Definitely not image types
  if(strcmp(extension, "json")==0 || strcmp(extension, "JSON")==0 )
    return ExtJson;
  if(strcmp(extension, "txt")==0 || strcmp(extension, "TXT")==0)
    return ExtTxt;
  if(strcmp(extension, "html")==0 || strcmp(extension, "HTML")==0 )
    return ExtHtml;
  if(strcmp(extension, "rtf")==0 || strcmp(extension, "RTF")==0 )
    return ExtRtf;
  if(strcmp(extension, "oldf")==0 || strcmp(extension, "OLDF")==0)
    return ExtOldf;
  if(strcmp(extension, "otdl")==0 || strcmp(extension, "OTDL")==0)
    return ExtOtdl;

  return ExtImproperValue;
}
  

// =======================================================================================
