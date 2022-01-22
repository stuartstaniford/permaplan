// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// Access permaserv from within permaplan.  This implements the client side of the 
// permaserv api, and also implements caching of answers from permaserv so we don't 
// bug it too often

#include "HttpPermaservClient.h"


// =======================================================================================
/// @brief Constructor

HttpPermaservClient::HttpPermaservClient(void)
{
}


// =======================================================================================
/// @brief Destructor

HttpPermaservClient::~HttpPermaservClient(void)
{
}


// =======================================================================================
/// @brief Get a value from the API that has the form "Name: value\n"
/// 
/// @returns A float with the value for diffuse horizontal irradiance
/// @param lat Latitude of the location we are querying about.
/// @param longt Longtitude of the location we are querying about.

bool HttpPermaservClient::getSingleValue(char* name, float lat, float longt, float retVal)
{
  return false;  
}


// =======================================================================================
/// @brief Get the diffuse horizontal irradiance value at a given location
/// 
/// @returns A float with the value for diffuse horizontal irradiance
/// @param lat Latitude of the location we are querying about.
/// @param longt Longtitude of the location we are querying about.
 
float HttpPermaservClient::getDIFValue(float lat, float longt)
{
  float retVal;
  if(getSingleValue("DIF", lat, longt, retVal))
    return retVal;
  else
   {
    // LOG ERROR HERE!!!
    return 0.0f;
   }
}


// =======================================================================================
/// @brief Get the direct normal irradiance value at a given location
/// 
/// @returns A float with the value for direct normal irradiance
/// @param lat Latitude of the location we are querying about.
/// @param longt Longtitude of the location we are querying about.
 
float HttpPermaservClient::getDNIValue(float lat, float longt)
{
  return 0.0f;
}


// =======================================================================================
