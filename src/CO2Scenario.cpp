// Copyright Staniford Systems.  All Rights Reserved.  March 2021 -
// Class for pulling a CO2 scenario out of the JSON file and providing it to the
// rest of the code as needed.

#include "CO2Scenario.h"
#include "Logging.h"
#include "Global.h"
#include "loadFileToBuf.h"

// =======================================================================================
// Initialize static variables

using namespace rapidjson;

bool      CO2Scenario::jsonLoaded = false;
char*     CO2Scenario::fileName   = (char*)"co2-scenarios.json";
Document  CO2Scenario::doc;


// =======================================================================================
/// @brief Constructor for the scenario.  
/// 
/// We are associated with a single scenario that is selected from a JSON file database.
/// @param scenarioName A char* string with the name of the particular scenario we are
/// associated with.  The scenario file is only loaded once per program instantiation.


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
  N = doc[scenarioName].Size();
  for(i=0; i< N; i++)
    (*this)[doc[scenarioName][i][0].GetFloat()] = doc[scenarioName][i][1].GetFloat();
}


// =======================================================================================
/// @brief Destructor

CO2Scenario::~CO2Scenario(void)
{
}


// =======================================================================================
/// @brief Get the concentration at a particular time.  
/// 
/// The function will interpolate between the particular values found in the CO2 file.
/// Note that we use geometric interpolation not linear, as this function is not expected 
/// to be performance critical.
/// @returns A float with the CO2 concentration (in ppm).
/// @param year A floating point specification of the year for which concentration is
/// requested.

float CO2Scenario::getConcentration(float year)
{
  auto iter = lower_bound(year);
  float yearAbove = iter->first;
  float concAbove = iter->second;
  iter--;
  float yearBelow = iter->first;
  float concBelow = iter->second;
  
  return powf( powf(concAbove, year-yearBelow) * powf(concBelow, yearAbove-year),
                                                      1.0f/(yearAbove-yearBelow));
}


// =======================================================================================
/// @brief Load the CO2 scenario file
///
/// Helper function called to load the scenario file into memory the first time we need
/// it.  


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

