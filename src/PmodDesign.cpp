// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Main object that encapsulates the design of a given piece of land.  This is basically
// the contents of the main json file given via -d.  Generally the rest of the command
// line is captured in PmodConfig, not in here.

#include <stdexcept>
#include <err.h>
#include "PmodDesign.h"
#include "loadFileToBuf.h"


using namespace rapidjson;

// =======================================================================================
// Constructor: parse the json file

PmodDesign::PmodDesign(void)
{
  unsigned bufSize;
  const PmodConfig& config = PmodConfig::getConfig();
  char* buf = loadFileToBuf(config.designFileName, &bufSize);
  
  ParseResult ok = doc.ParseInsitu<kParseCommentsFlag>(buf);
  if (!ok)
   {
    fprintf(stderr, "JSON parse error: %s (%u)",
            GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
    exit(1);
   }
  if(!doc.IsObject())
    err(-1, "Base of design file %s is not JSON object.\n", config.designFileName);
}

// =======================================================================================
// Destructor

PmodDesign::~PmodDesign()
{

}


