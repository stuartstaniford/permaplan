// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// This class keeps track of different classes of memory for leak detection and general
// memory management.


#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#include "HttpDebug.h"

#define MEMORY_TRACKING_ON

// =======================================================================================
// Class variable initialization


class MemoryTracker
{
public:
  
  // Instance variables - public
  static long     arrowMemory;
  static long     bezierPatchMemory;
  static long     boxMemory;

  // Member functions - public
  MemoryTracker(void);
  ~MemoryTracker(void);
  static bool diagnosticHTML(HttpDebug* serv);

  
private:
  
  // Instance variables - private
  
  // Member functions - private
  MemoryTracker(const MemoryTracker&);                 // Prevent copy-construction
  MemoryTracker& operator=(const MemoryTracker&);      // Prevent assignment

};

#ifdef MEMORY_TRACKING_ON
#define incrementArrowMemory(x) MemoryTracker::arrowMemory += (x)
#define incrementBezierPatchMemory(x) MemoryTracker::bezierPatchMemory += (x)
#define incrementBoxMemory(x) MemoryTracker::boxMemory += (x)
#else
#define incrementArrowMemory(x)
#define incrementBezierPatchMemory(x)
#define incrementBoxMemory(x)
#endif



#endif




