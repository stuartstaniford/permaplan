// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Class to model/manipulate OLDF, OTDL, etc version numbers.

#define VERSION_IMPLEMENTATION

#include "Version.h"
#include "Global.h"
#include <cstdio>
#include <err.h>

// =======================================================================================
// Static variables etc

char Version::buf[64];
bool externVersionsSet = false;
Version currentOLDFVersion;
Version currentOTDLVersion;


// =======================================================================================
/// @brief Utility function to set globally useful version values.

void setExternVersions(void)
{
  currentOLDFVersion.set(0u, 0u, 2u);
  currentOTDLVersion.set(0u, 0u, 1u);
  externVersionsSet = true;
}


// =======================================================================================
/// @brief Constructor
/// @param major The major version number
/// @param minor The minor version number
/// @param increment The increment (Ie version numbers are like 1.2.17, meaning the 17th
/// increment of version 1.2).

Version::Version(unsigned major, unsigned minor, unsigned increment)
{
  version[0] = major;
  version[1] = minor;
  version[2] = increment;
}


// =======================================================================================
/// @brief Destructor

Version::~Version(void)
{
}


// =======================================================================================
/// @brief Set function, for when we have to change the version after creation.
/// @param major The major version number
/// @param minor The minor version number
/// @param increment The increment (Ie version numbers are like 1.2.17, meaning the 17th
/// increment of version 1.2).

void Version::set(unsigned major, unsigned minor, unsigned increment)
{
  version[0] = major;
  version[1] = minor;
  version[2] = increment;
}


// =======================================================================================
/// @brief Print in our static buffer a JSON array version
/// @returns A ptr to the JSON array text in our static buffer.
/// Note this is not thread-safe.

char* Version::printJSON(void)
{
  sprintf(buf, "[%u, %u, %u]", version[0], version[1], version[2]);
  return buf;
}


// =======================================================================================
/// @brief Assignment operator

void Version::operator=(const Version& inward)
{
  set(inward.version[0], inward.version[1], inward.version[2]);
}


// =======================================================================================
/// @brief Subscript operator so, eg, you can say ver[1] and get the minor number.

unsigned Version::operator[] (int i)
{
  if(i>=0 && i< 3)
    return version[i];
  err(-1, "Bad access to version [] operator.");
}


// =======================================================================================
