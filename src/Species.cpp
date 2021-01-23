// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is a container for the factors governing the evolution of a particular
// species (mainly extracted from an OTDL JSON object).

#include "Species.h"
#include <err.h>


// =======================================================================================
// Constructors.

Species::Species(void)
{
}


// =======================================================================================
// Destructor

Species::~Species(void)
{
}


        
// =======================================================================================
// Tell callers our name at runtime.

const char* Species::objectName(void)
{
  static char* name = (char*)"Species";
  return name;
}


// =======================================================================================
// We assume we are part of a table of visual objects and we just contribute one row
// about this particular object.

bool Species::diagnosticHTML(HttpDebug* serv)
{
  serv->addResponseData("<tr><td>Species</td>");
  return true;
}


// =======================================================================================
