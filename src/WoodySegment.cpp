// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class manages a segment of a trunk/branch/stem of a woody plant or tree.  It
// models the heartwood/sapwood/bark, etc.

#include "WoodySegment.h"
#include <err.h>


// =======================================================================================
// Constructors.

WoodySegment::WoodySegment(void)
{
}


// =======================================================================================
// Destructor

WoodySegment::~WoodySegment(void)
{
}


        
// =======================================================================================
// Tell callers our name at runtime.

const char* WoodySegment::objectName(void)
{
  static char* name = (char*)"WoodySegment";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.

bool WoodySegment::diagnosticHTML(HttpDebug* serv)
{
  
  serv->addResponseData("<tr><td>WoodySegment</td>");
  return true;
}


// =======================================================================================
