// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Main object that encapsulates the design of a given piece of land.  This is basically
// the contents of the main OLDF file given via -d.  Generally the rest of the command
// line is captured in PmodConfig, not in here.
// See docs/open-landscape-description-format.md for the file format.


#include "PmodDesign.h"
#include <stdexcept>
#include <err.h>
#include <math.h>
#include "loadFileToBuf.h"
#include "Logging.h"
#include "GlobalMacros.h"
#include "JSONStructureChecker.h"

using namespace rapidjson;

// =======================================================================================
// Constructor: parse the json file

JSONStructureChecker::JSONStructureChecker(void)
{
}



// =======================================================================================
// Destructor

JSONStructureChecker::~JSONStructureChecker()
{

}


// =======================================================================================
