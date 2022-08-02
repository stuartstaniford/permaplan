// Copyright Staniford Systems.  All Rights Reserved.  August 2022 -
// A class to manage the permaplan client side process of getting soil profiles
// from the SoilDatabase in permaserv, and making them available for plant growth
// algorithms as needed.

#include "SoilDatabaseClient.h"
#include "HttpPermaservClient.h"
#include "SoilProfile.h"


// =======================================================================================
/// @brief Constructor
///
/// @param httpPermCli a reference to the instance of HttpPermaservClient we will use
/// to communicate with permaserv (where we want to talk to the SoilDatabase).

SoilDatabaseClient::SoilDatabaseClient(HttpPermaservClient& httpPermCli):
                                                            httpPermClient(httpPermCli)
{
}


// =======================================================================================
/// @brief Destructor

SoilDatabaseClient::~SoilDatabaseClient(void)
{
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

SoilProfile* SoilDatabaseClient::getSoil(float latitude, float longtitude)
{
  return NULL;
}


// =======================================================================================