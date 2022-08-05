// Copyright Staniford Systems.  All Rights Reserved.  August 2022 -
// A class to manage the permaplan client side process of getting soil profiles
// from the SoilDatabase in permaserv, and making them available for plant growth
// algorithms as needed.

#include "SoilDatabaseClient.h"
#include "HttpPermaservClient.h"
#include "SoilProfile.h"
#include "PmodDesign.h"
#include "Global.h"
#include "Logging.h"


// =======================================================================================
// Initialization of variables, etc

SoilDatabaseClient* SoilDatabaseClient::theSoilDbClient = NULL;


// =======================================================================================
/// @brief Constructor
///
/// @param httpPermCli a reference to the instance of HttpPermaservClient we will use
/// to communicate with permaserv (where we want to talk to the SoilDatabase).

SoilDatabaseClient::SoilDatabaseClient(HttpPermaservClient& httpPermCli):
                                                            httpPermClient(httpPermCli)
{
  if(theSoilDbClient)
    return;
  else
    theSoilDbClient = this;
}


// =======================================================================================
/// @brief Destructor

SoilDatabaseClient::~SoilDatabaseClient(void)
{
}


// =======================================================================================
/// @brief Fetch the available soil profiles from the SoilDatabase in Permaserv.
/// @returns True if we could fetch a non-zero number of profiles, false otherwise.

using namespace rapidjson;

bool SoilDatabaseClient::getProfilesFromSoilDatabase(void)
{
  // Figure out the area where we need to ask for soil profiles
  PmodDesign& design = PmodDesign::getDesign();
  float loLat, hiLat, loLong, hiLong;
  design.boundary.latLongRange(loLat, hiLat, loLong, hiLong);
  scaleLatLongRectangle(2.0f, loLat, hiLat, loLong, hiLong);
  
  // Send the request to permaserv
  unless(httpPermClient.getSoilProfiles(loLat, hiLat, loLong, hiLong))
   {
    LogPermaservClientErrors("Failed to get soil profile response in "            
                                      "SoilDatabaseClient::getProfilesFromSoilDatabase.\n");
    return false;
   }
  
  // Parse the returned document to extract the soil profiles.  Note we are a friend
  // of HttpPermaservClient so we can access it's private doc variable - makes more 
  // sense to keep the detailed knowledge of soil profile JSON structure here.
  
  return true;
}


// =======================================================================================
/// @brief Get a pointer to the soil profile at a particular <lat,long>.
/// 
/// This is called by other code in permaserv that needs soil information.  It may
/// trigger fetching further soil information from permaserv if that has not been done
/// yet.  However, it is anticipated that most calls will return quickly with a pointer
/// to soil information we already have.
/// @returns A pointer to a SoilProfile that the caller can use when soil properties
/// are required (eg for input to plant growth algorithms).
/// @param latitude The float latitude of where we are hoping to find out soil info for
/// @param longtitude The float longtitude of where we are hoping to find out soil info for
/// @todo Currently we just return the first and only element of the array.  Ultimately
/// we should handle complex cases of heterogeneity in the soil across the planned design.

SoilProfile* SoilDatabaseClient::getSoil(float latitude, float longtitude)
{
  if(soilSamples.size() > 0)
    return soilSamples[0];
  else
   {
    if(getProfilesFromSoilDatabase() && soilSamples.size() > 0)
      return soilSamples[0];
    else
      return NULL;
   }
}


// =======================================================================================
