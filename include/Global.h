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
#define isWhiteSpace(x) ( (x)==' ' || (x)=='\t' || (x)=='\n' || (x)=='\r' || (x)=='\f' )

#define MULTI_THREADED_SIMULATION

// Time/date related macros

#define IsLeapYear(X) ((X)%4?0:((X)%100?1:((X)%400?0:1)))
#define yearDays(Year, Month, Day) (IsLeapYear(Year)?(yearDaysLeap[(Month)-1]+((Day)-1)):\
                                                    (yearDaysNonLeap[(Month)-1]+((Day)-1)))
#define DaysInYear(X) (IsLeapYear(X)?366:365)

// Latitude and longtitude related macros
#define latitudeInRange(f) ( ( (f) >= -90.0f) && ((f) <= 90.0f) )
#define longtitudeInRange(f) ( ( (f) >= -180.0f) && ((f) <= 180.0f) )

// Prevent assignment and copy construction of class "T".
#define PreventAssignAndCopyConstructor(T) private: T(const T&); T&operator=(const T&);

// Constants
#define MAX_USERNAME_LEN  32
#define EPSILON           0.000000001f


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
// Inline utility functions

/// @brief Function to rescale a float interval about its centroid.
inline void scaleInterval(const float& scale, float& lo, float& hi)
{
  float center = (lo+hi)/2.0f;
  float halfExtent = scale*(hi-lo)/2.0f;
  lo = center - halfExtent;
  hi = center + halfExtent;
}

/// @brief Function to rescale a rectangle defined by lat/long coords about it's centroid.
inline void scaleLatLongRectangle(const float& scale, float& loLat, float& hiLat, 
                                                            float& loLong, float& hiLong)
{
  scaleInterval(scale, loLat, hiLat);
  scaleInterval(scale, loLong, hiLong);  
}


// =======================================================================================
// Utility function prototypes

void initGlobals(int nThreads = 0);
void hexdump(void *ptr, int buflen, char* outBuf);
bool extractColonVecN(char* path, int N, float* dest);
inline bool extractColonVec4(char* path, vec4 dest)
              {return extractColonVecN(path, 4, dest);}
inline bool extractColonVec3(char* path, vec3 dest)
              {return extractColonVecN(path, 3, dest);}
inline bool extractColonVec2(char* path, vec2 dest)
              {return extractColonVecN(path, 2, dest);}
inline bool extractColonFloat(char* path, float* value)
              {return extractColonVecN(path, 1, value);}
bool checkLatLong(float* values);
bool checkLatLongRegion(float* values);
FileExtension findExtension(const char* fileName);
char* errorInNumericCharacterField(char* field);
char* errorInFloat(float f);


// =======================================================================================
// Extern definitions of global variables here
#ifndef GLOBAL_IMPLEMENTATION

class TaskQueueFarm;
extern TaskQueueFarm* threadFarm;
extern int yearDaysNonLeap[12];
extern int yearDaysLeap[12];
#endif


// =======================================================================================
// Utility function declarations

void  getCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius);
void  updateCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius);


// =======================================================================================

#endif
