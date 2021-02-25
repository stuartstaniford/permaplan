// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// This class keeps track of different classes of memory for leak detection and general
// memory management.


#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#include "HttpDebug.h"

// =======================================================================================
// Class variable initialization


class MemoryTracker
{
public:
  
  // Instance variables - public
  
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

#endif




