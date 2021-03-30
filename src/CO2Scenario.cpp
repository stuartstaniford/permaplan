// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Class for pulling a CO2 scenario out of the JSON file and providing it to the
// rest of the code as needed.

#include "CO2Scenario.h"
#include "Logging.h"
#include "GlobalMacros.h"
#include "loadFileToBuf.h"

// =======================================================================================
// Initialize static variables

using namespace rapidjson;

bool      CO2Scenario::jsonLoaded = false;
char*     CO2Scenario::fileName   = (char*)"co2-scenarios.json";
Document  CO2Scenario::doc;


// =======================================================================================
// Constructor

CO2Scenario::CO2Scenario(char* scenarioName)
{
  unless(jsonLoaded)   
   {
    // load the scenarios from the JSON file only one the first call
    loadScenarioFile();
    jsonLoaded = true;
   }
  
  int i, N = doc["Historical"].Size();
  for(i=0; i< N; i++)
    (*this)[doc["Historical"][i][0].GetFloat()] = doc["Historical"][i][1].GetFloat();
}


// =======================================================================================
// Destructor

CO2Scenario::~CO2Scenario(void)
{
}


// =======================================================================================
// Load CO2 scenario file


void CO2Scenario::loadScenarioFile(void)
{
  unsigned  bufSize;
  char*     fileName = (char*)"co2-scenarios.json";
  
  char* buf = loadFileToBuf(fileName, &bufSize);

  ParseResult ok = doc.ParseInsitu<kParseCommentsFlag>(buf);
  unless(ok)
   {
    fprintf(stderr, "JSON parse error on co2 scenarios file %s: %s (%u)\n",
                          fileName, GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
    exit(1);
   }

}


// =======================================================================================

