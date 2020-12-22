// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Main object that encapsulates the design of a given piece of land.  This is basically
// the contents of the main OLDF file given via -d.  Generally the rest of the command
// line is captured in PmodConfig, not in here.
// See docs/open-landscape-description-format.md for the file format.


#include <stdexcept>
#include <err.h>
#include "PmodDesign.h"
#include "loadFileToBuf.h"


using namespace rapidjson;

PmodDesign* PmodDesign::design = NULL;

// =======================================================================================
// Constructor: parse the json file

PmodDesign::PmodDesign(void)
{
  // Constructor should only be called once at startup.  Everyone else gets us via
  // getDesign()
  if(design)
    return;
  else
    design = this;
  unsigned bufSize;
  const PmodConfig& config = PmodConfig::getConfig();
  char* buf = loadFileToBuf(config.designFileName, &bufSize);
  
  ParseResult ok = doc.ParseInsitu<kParseCommentsFlag>(buf);
  if (!ok)
   {
    fprintf(stderr, "JSON parse error: %s (%u)\n",
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


