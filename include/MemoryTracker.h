// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -

#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#define MEMORY_TRACKING_ON


// =======================================================================================
// Forward declarations

class HttpDebug;


// =======================================================================================
/// @brief This class keeps track of different classes of memory for leak detection 
/// and general memory management.
/// @todo The memory tracking is very incomplete and what there is, is probably in 
/// disrepair.  This needs sustained attention and integration into the test script 
/// framework at some point.

class MemoryTracker
{
 public:
  
  // Instance variables - public
  static long     arrowMemory;
  static long     bezierPatchMemory;
  static long     boxMemory;
  static long     triangleBufferMemory;
  static long     treeMemory;

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


// =======================================================================================
// Memory tracking macros

#ifdef MEMORY_TRACKING_ON

#define incrementArrowMemory(x) MemoryTracker::arrowMemory += (x)
#define incrementBezierPatchMemory(x) MemoryTracker::bezierPatchMemory += (x)
#define incrementBoxMemory(x) MemoryTracker::boxMemory += (x)
#define incrementTriangleBufferMemory(x) MemoryTracker::triangleBufferMemory += (x)
#define incrementTreeMemory(x) MemoryTracker::treeMemory += (x)

#else

#define incrementArrowMemory(x)
#define incrementBezierPatchMemory(x)
#define incrementBoxMemory(x)
#define incrementTriangleBufferMemory(x)
#define incrementTreeMemory(x)

#endif


// =======================================================================================

#endif




