// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Main object that encapsulates the design of a given piece of land.  This is basically
// the contents of the main OLDF file given via -d.  Generally the rest of the command
// line is captured in PmodConfig, not in here.
// See docs/open-landscape-description-format.md for the file format.


#include <stdexcept>
#include <err.h>
#include "PmodDesign.h"
#include "loadFileToBuf.h"
#include "Logging.h"


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
    fprintf(stderr, "JSON parse error on OLDF file: %s (%u)\n",
            GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
    exit(1);
   }
  if(!doc.IsObject())
    err(-1, "Base of OLDF file %s is not JSON object.\n", config.designFileName);
  if(!validateOLDF())
    err(-1, "Invalid OLDF file %s - see log for details\n", config.designFileName);

}


// =======================================================================================
// Function to check the structure of the OLDF introductoryData object.

bool PmodDesign::validateIntroductoryData(void)
{
  bool              retVal              = true;
  Value&            introductoryData    = doc["introductoryData"];
  int               expectedVersion[3]  = {0,0,2};
  const PmodConfig& config              = PmodConfig::getConfig();

  // spaceUnits
  if(introductoryData.HasMember("spaceUnits") && introductoryData["spaceUnits"].IsString())
   {
    const char* token = introductoryData["spaceUnits"].GetString();
    if(!(strcmp(token, "feet") == 0 || strcmp(token, "meters") == 0 ))
     {
      LogOLDFValidity("Bad introductoryData:spaceUnits %s, in OLDF file %s\n",
                                                            token, config.designFileName);
      retVal = false;
     }
    else
     LogOLDFDetails("spaceUnits are %s in OLDF file %s\n", token, config.designFileName);
   }
  else
   {
    LogOLDFValidity("No introductoryData:spaceUnits token in OLDF file %s\n",
                                                                    config.designFileName);
    retVal = false;
   }

  // version
  if(introductoryData.HasMember("version") && introductoryData["version"].IsArray())
   {
    Value& versionArray = introductoryData["version"];
    if(versionArray.Size() == 3)
     {
      bool versionGood = true;
      for (int i = 0; i < versionArray.Size(); i++)
       {
        if(!(versionArray[i].IsInt() && versionArray[i].GetInt() == expectedVersion[i]))
         {
          versionGood = false;
          LogOLDFValidity("introductoryData:version array is not %d at pos %d in OLDF file %s\n",
                                                    expectedVersion[i], i, config.designFileName);
         }
       }
      if(versionGood)
        LogOLDFDetails("version is [%d,%d,%d] in OLDF file %s\n", expectedVersion[0],
                                  expectedVersion[1], expectedVersion[2], config.designFileName);
      else
        retVal = false;
     }
    else
     {
      LogOLDFValidity("introductoryData:version array is wrong size %d in OLDF file %s\n",
                                                    versionArray.Size(), config.designFileName);
     }
   }
  else
   {
    LogOLDFValidity("No introductoryData:version array in OLDF file %s\n", config.designFileName);
    retVal = false;
   }

  return retVal;
}


// =======================================================================================
// Function to check the structure of the OLDF object after it has been parsed out of the
// file (ie it is at least known to be valid JSON at this point, now we want to know if
// it's valid OLDF.  Do a lot of logging of the structure if so configured.

bool PmodDesign::validateOLDF(void)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();

  if(!(doc.HasMember("introductoryData") && doc["introductoryData"].IsObject()))
   {
    LogOLDFValidity("No introductoryData in OLDF file %s\n", config.designFileName);
    retVal = false;
   }
  else
    retVal &= validateIntroductoryData();
  
  return retVal;
}


// =======================================================================================
// Destructor

PmodDesign::~PmodDesign()
{

}


// =======================================================================================
