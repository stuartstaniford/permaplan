// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// This class keeps track of different classes of memory for leak detection and general
// memory management.

#include "MemoryTracker.h"
#include "HttpDebug.h"
#include <cstdio>
#include <err.h>


// =======================================================================================
// Static variables

long MemoryTracker::arrowMemory           = 0;
long MemoryTracker::bezierPatchMemory     = 0;
long MemoryTracker::boxMemory             = 0;
long MemoryTracker::triangleBufferMemory  = 0;
long MemoryTracker::treeMemory            = 0;


// =======================================================================================
/// @brief Constructor

MemoryTracker::MemoryTracker(void)
{

}


// =======================================================================================
/// @brief Destructor

MemoryTracker::~MemoryTracker(void)
{
}



// =======================================================================================
/// @brief Provide info on memory usage to the HTTP debug interface
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool MemoryTracker::diagnosticHTML(HttpDebug* serv)
{
  serv->startResponsePage("Memory Usage");
  
  httPrintf("<center>\n");
  serv->startTable();
  httPrintf("<tr><th>Category</th><th>Usage (kB)</th></tr>\n");

  httPrintf("<tr><th>Arrow Memory</th><th>%.1f</th></tr>\n",        arrowMemory/1024.0);
  httPrintf("<tr><th>Bezier Patch Memory</th><th>%.1f</th></tr>\n",
                                                                bezierPatchMemory/1024.0);
  httPrintf("<tr><th>Block(box) Memory</th><th>%.1f</th></tr>\n",   boxMemory/1024.0);
  httPrintf("<tr><th>Triangle Buffer Memory</th><th>%.1f</th></tr>\n",
                                                              triangleBufferMemory/1024.0);
  httPrintf("<tr><th>Tree Memory</th><th>%.1f</th></tr>\n",   treeMemory/1024.0);

  httPrintf("</table></center><hr>\n");
  serv->endResponsePage();
  return true;
}


// =======================================================================================
