// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// Access permaserv from within permaplan.  This implements the client side of the 
// permaserv api, and also implements caching of answers from permaserv so we don't 
// bug it too often

#include "HttpPermaservClient.h"
#include "Global.h"
#include <err.h>
#include <string.h>


// =======================================================================================
/// @brief Constructor

HttpPermaservClient::HttpPermaservClient(unsigned short sPort):
                                              servPort(sPort)
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

#define FULL_URL_BUFSIZE 128
#define SINGLE_VAL_BUFSIZE 128

bool HttpPermaservClient::getSingleValue(char* url, char* name, float lat, float longt, float& retVal)
{
  char fullUrl[FULL_URL_BUFSIZE];
  char recvBuf[SINGLE_VAL_BUFSIZE];
  
  int printRet = snprintf(fullUrl, FULL_URL_BUFSIZE, "http://127.0.0.1:%u/%s?%f,%f", 
                                                                  servPort, url, lat, longt);
  if(printRet < 1 || printRet >= FULL_URL_BUFSIZE)
    err(-1, "Overflow in HttpPermaservClient::getSingleValue");
  
  unless(fetchBuffer(fullUrl, recvBuf, SINGLE_VAL_BUFSIZE))
   {
    // NEED LOGGING that we couldn't get a response from the server
    return false; 
   }
  int l = strlen(name);
  
  unless(strncmp(name, recvBuf, l) == 0 && recvBuf[l] == ':')
   {
    // NEED LOGGING that format wasn't right
    return false;
   }
  
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
  if(getSingleValue((char*)"dif", (char*)"DIF", lat, longt, retVal))
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
