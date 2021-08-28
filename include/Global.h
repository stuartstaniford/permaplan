// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// A place for macros that we want to be able to use in many locations and also to define
// global variables used throughout the program.


#ifndef GLOBAL_MACRO_DEFNS_H
#define GLOBAL_MACRO_DEFNS_H
#include <cglm/cglm.h>

// =======================================================================================
// Global macro definitions

#define unless(X) if(!(X))
#define until(X) while(!(X))
#define EPSILON 0.000000001f
#define isWhiteSpace(x) ( (x)==' ' || (x)=='\t' || (x)=='\n' || (x)=='\r' || (x)=='\f' )

#define MULTI_THREADED_SIMULATION

// =======================================================================================
// Useful enums

enum FileExtension
{
  ExtImproperValue,
  ExtNoExtension,
  ExtEndsInDot,
  ExtJpeg,
  ExtPng,
  ExtGif,
  ExtBmp,
  ExtJson,
  ExtTxt,
  ExtHtml,
  ExtRtf,
  ExtOldf,
  ExtOtdl,
};


// =======================================================================================
// Function prototypes

void initGlobals(int nThreads);
bool extractColonVecN(char* path, int N, float* dest);
inline bool extractColonVec4(char* path, vec4 dest)
              {return extractColonVecN(path, 4, dest);}
inline bool extractColonVec3(char* path, vec3 dest)
              {return extractColonVecN(path, 3, dest);}
inline bool extractColonVec2(char* path, vec2 dest)
              {return extractColonVecN(path, 2, dest);}
inline bool extractColonFloat(char* path, float* value)
              {return extractColonVecN(path, 1, value);}
FileExtension findExtension(const char* fileName);
char* errorInNumericCharacterField(char* field);
char* errorInFloat(float f);


// =======================================================================================
// Extern definitions of global variables here
#ifndef GLOBAL_IMPLEMENTATION

class TaskQueueFarm;
extern TaskQueueFarm* threadFarm;

#endif

#endif


// =======================================================================================
// Utility function declarations

void  getCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius);
void  updateCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius);


