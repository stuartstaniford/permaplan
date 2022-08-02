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
