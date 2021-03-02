// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Class to model/manipulate OLDF, OTDL, etc version numbers.

#define VERSION_IMPLEMENTATION

#include "Version.h"
#include "GlobalMacros.h"
#include <cstdio>
#include <err.h>

char Version::buf[64];

// =======================================================================================
// Utility function to set globally useful version values.

bool externVersionsSet = false;
Version currentOLDFVersion;
Version currentOTDLVersion;

void setExternVersions(void)
{
  currentOLDFVersion.set(0u, 0u, 2u);
  currentOTDLVersion.set(0u, 0u, 1u);
  externVersionsSet = true;
}


// =======================================================================================
// Constructors

Version::Version(unsigned major, unsigned minor, unsigned increment)
{
  if(!externVersionsSet)
    setExternVersions();
  version[0] = major;
  version[1] = minor;
  version[2] = increment;
}


// =======================================================================================
// Destructor

Version::~Version(void)
{
}


// =======================================================================================
// Set function, for when we have to change it after creation.

void Version::set(unsigned major, unsigned minor, unsigned increment)
{
  version[0] = major;
  version[1] = minor;
  version[2] = increment;
}


// =======================================================================================
// Print in our static buffer a JSON array version, and return a ptr to it.
// Not thread-safe.

char* Version::printJSON(void)
{
  sprintf(buf, "[%u, %u, %u]", version[0], version[1], version[2]);
  return buf;
}


// =======================================================================================
// Assignment operator

void Version::operator=(const Version& inward)
{
  set(inward.version[0], inward.version[1], inward.version[2]);
}


// =======================================================================================
// Subscript operator

unsigned Version::operator[] (int i)
{
  if(i>=0 && i< 3)
    return version[i];
  err(-1, "Bad access to version [] operator.");
}


// =======================================================================================
