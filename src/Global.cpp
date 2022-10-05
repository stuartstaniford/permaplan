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

// Calendar related globals
int monthDaysLeap[11] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30};
int monthDaysNonLeap[11] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};
int yearDaysNonLeap[12];
int yearDaysLeap[12];


// =======================================================================================
// Initialization of the global variables defined in here.

void initGlobals(int nThreads)
{
#ifdef MULTI_THREADED_SIMULATION
  threadFarm = new TaskQueueFarm(nThreads, (char*)"treeSimFarm");  
#endif

  yearDaysNonLeap[0] = yearDaysLeap[0] = 0;
  for(int i=1; i<12; i++)
   {
    yearDaysNonLeap[i] = yearDaysNonLeap[i-1] + monthDaysNonLeap[i-1];
    yearDaysLeap[i] = yearDaysLeap[i-1] + monthDaysLeap[i-1];
   }
}


// =======================================================================================
// Extract a ':' separated vector from a (URL) string.

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
// Utility function to get two vectors perpendicular to a specified one.
// Eg if sides == 8 the cross section of the TruncatedCone which points in the dir
// direction looks like this (dir points into the screen):
/*
     ^
     | f1 direction
 
   -----
  /     \
 /       \
|         |  ---->
|         |  f2 direction
 \       /
  \     /
   -----
*/

void getCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius)
{
  vec3 f0; // starting place, will be z-axis unless dir is parallel when we use x-axis.

  if(fabs(dir[0]) < EPSILON && fabs(dir[1]) < EPSILON)
   {
    f0[0] = 1.0f;
    f0[1] = f0[2] = 0.0f;
   }
  else
   {
    f0[0] = f0[1] = 0.0f;
    f0[2] = 1.0f;
   }

  glm_vec3_cross(f0, dir, f2);
  glm_vec3_cross(dir, f2, f1);
  glm_vec3_scale_as(f1, radius, f1);
  glm_vec3_scale_as(f2, radius, f2);
}


// =======================================================================================
// This function is used te ensure the stability of f1, f2 as we go up the tube, we use the 
// last f1,f2 as the starting point for the next set. 


void updateCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius)
{
  glm_vec3_cross(dir, f2, f1);
  glm_vec3_cross(f1, dir, f2);
  glm_vec3_scale_as(f1, radius, f1);
  glm_vec3_scale_as(f2, radius, f2);
}


// =======================================================================================
// Function to determine the endianness of the machine we are running on 
// Stolen from https://developer.ibm.com/articles/au-endianc/

int endian(void) 
{
    int i = 1;
    char* p = (char*)&i;

    if (p[0] == 1)
        return LITTLE_ENDIAN;
    else
        return BIG_ENDIAN;
}


// =======================================================================================
