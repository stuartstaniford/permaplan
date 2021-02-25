
// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// This class keeps track of different classes of memory for leak detection and general
// memory management.

#include "MemoryTracker.h"
#include <cstdio>
#include <err.h>


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
    
  httPrintf("</table></center><hr>\n");
  serv->endResponsePage();
  return true;
}


// =======================================================================================
