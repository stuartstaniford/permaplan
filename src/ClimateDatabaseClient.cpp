// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// Fetches climate information from permaserv.  Gets, for our location, information 
// about the current/recent climate, as well as projections of future changed climate.

#include "ClimateDatabaseClient.h"
#include "HttpPermaservClient.h"
#include "PmodDesign.h"
#include "Global.h"
#include "Logging.h"


// =======================================================================================
/// @brief Constructor

ClimateDatabaseClient::ClimateDatabaseClient(HttpPermaservClient& httpPermCli):
                                                              httpPermClient(httpPermCli)
{
}


// =======================================================================================
/// @brief Destructor

ClimateDatabaseClient::~ClimateDatabaseClient(void)
{
}


// =======================================================================================
/// @brief Fetch the available climate data from the ClimateDatabase in Permaserv.
/// @returns True if we could fetch data, false otherwise.
/// @todo Climate data parsing is a stub right now.

using namespace rapidjson;

bool ClimateDatabaseClient::getClimateDataFromDatabase(void)
{
  // Figure out the area where we need to ask for soil profiles
  PmodDesign& design = PmodDesign::getDesign();
  float loLat, hiLat, loLong, hiLong;
  design.boundary.latLongRange(loLat, hiLat, loLong, hiLong);
  
  // Send the request to permaserv
  unless(httpPermClient.getClimateData((loLat+hiLat)/2.0f, (loLong+hiLong)/2.0f))
   {
    LogPermaservClientErrors("Failed to get climate data response in "            
                                      "ClimateDatabaseClient::getClimateDataFromDatabase.\n");
    return false;
   }
  
  // Parse the returned document to extract the climate data.  Note we are a friend
  // of HttpPermaservClient so we can access its private doc variable - makes more 
  // sense to keep the detailed knowledge of soil profile JSON structure here.
  Document& climateJson = httpPermClient.doc;
  if(climateJson.HasMember("Suppress compiler warning"))
    return false;
         
  return true;
}


// =======================================================================================
