
// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// This class keeps track of different classes of memory for leak detection and general
// memory management.

#include "MemoryTracker.h"
#include <cstdio>
#include <err.h>

unsigned long   MemoryTracker::arrowMemory          = 0u;
unsigned long   MemoryTracker::bezierPatchMemory    = 0u;

// =======================================================================================
// Constructor

MemoryTracker::MemoryTracker(void)
{

}


// =======================================================================================
// Destructor

MemoryTracker::~MemoryTracker(void)
{
}



// =======================================================================================
// Provide info to the HTTP debug interface

bool MemoryTracker::diagnosticHTML(HttpDebug* serv)
{
  serv->startResponsePage("Memory Usage");
  
  httPrintf("<center>\n");
  serv->startTable();
  httPrintf("<tr><th>Category</th><th>Usage (kB)</th></tr>\n");

  httPrintf("<tr><th>Arrow Memory</th><th>%.1f</th></tr>\n", arrowMemory/1024.0);
  httPrintf("<tr><th>Bezier Patch Memory</th><th>%.1f</th></tr>\n", bezierPatchMemory/1024.0);

  httPrintf("</table></center><hr>\n");
  serv->endResponsePage();
  return true;
}


// =======================================================================================
