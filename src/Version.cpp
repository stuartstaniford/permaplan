// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Class to model/manipulate OLDF, OTDL, etc version numbers.

#include "Version.h"
#include "GlobalMacros.h"


// =======================================================================================
// Constructors

Version::Version(unsigned major, unsigned minor, unsigned increment)
{
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
