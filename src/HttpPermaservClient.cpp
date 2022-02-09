// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// Access permaserv from within permaplan.  This implements the client side of the 
// permaserv api, and also implements caching of answers from permaserv so we don't 
// bug it too often

#include "HttpPermaservClient.h"
#include "PmodConfig.h"
#include "Logging.h"
#include "Global.h"
#include "loadFileToBuf.h"
#include <err.h>
#include <string.h>
#include <sys/stat.h>


// =======================================================================================
// Static variables, etc

HttpPermaservClient*  HttpPermaservClient::theClient  = NULL;
char*                 cacheFileName                   = (char*)"client-cache.json";

using namespace rapidjson;

// =======================================================================================
/// @brief Constructor

HttpPermaservClient::HttpPermaservClient(void)
                                              
{
  // Constructor should only be called once at startup.  Everyone else gets us via
  // getPermaservClient()
  if(theClient)
    return;
  else
    theClient = this;

  servPort = PmodConfig::getConfig().permaservPort;
  
  unsigned bufSize;
  char* buf = NULL;
  struct stat cacheStatStruct;
  if(stat(cacheFileName, &cacheStatStruct) == 0)
   {
    // We have a file
    if(cacheStatStruct.st_mode & S_IFREG)
      buf = loadFileToBuf(cacheFileName, &bufSize);
   }
  else
   {
    // No file, leave buf NULL to signal that downfunction.
   }
  
  if(buf)
   {
    ParseResult ok = doc.ParseInsitu<kParseCommentsFlag>(buf);
    if (!ok)
     {
      fprintf(stderr, "JSON parse error on Permaserv client cache file %s: %s (%u)\n",
                      cacheFileName, GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
      exit(1);
     }
    if(!doc.IsObject())
      err(-1, "Base of cache file %s is not JSON object.\n", cacheFileName);
    cachePresent = true;
   }
  else
   {
    cachePresent = false;
    LogPermaservClientErrors("Cache file %s not found or not readable"
                             "in HttpPermaservClient constructor.\n", cacheFileName);
   }
}


// =======================================================================================
/// @brief Destructor

HttpPermaservClient::~HttpPermaservClient(void)
{
}


// =======================================================================================
/// @brief Get a value from the API that has the form "Name: value\n"
/// 
/// @returns True if we successfully found a value, false if we failed somehow (which
/// will be logged).
/// @param url The path of the url for this resource on the permaserv server
/// @param name The expected name of the value (before the ':') in the response.
/// @param lat Latitude of the location we are querying about.
/// @param longt Longtitude of the location we are querying about.
/// @param retVal A reference to a float to store the value found.

#define FULL_URL_BUFSIZE 128
#define SINGLE_VAL_BUFSIZE 128

bool HttpPermaservClient::getSingleValue(char* url, char* name, float lat, float longt, float& retVal)
{
  char fullUrl[FULL_URL_BUFSIZE];
  char recvBuf[SINGLE_VAL_BUFSIZE];
  
  int printRet = snprintf(fullUrl, FULL_URL_BUFSIZE, "http://127.0.0.1:%u/%s?%f,%f:", 
                                                                  servPort, url, lat, longt);
  if(printRet < 1 || printRet >= FULL_URL_BUFSIZE)
    err(-1, "Overflow in HttpPermaservClient::getSingleValue");
  
  unless(fetchBuffer(fullUrl, recvBuf, SINGLE_VAL_BUFSIZE))
   {
    LogPermaservClientErrors("Couldn't get response from %s "
                             "in HttpPermaservClient::getSingleValue.\n", fullUrl);
    return false; 
   }
  int l = strlen(name);
  
  unless(strncmp(name, recvBuf, l) == 0 && recvBuf[l] == ':')
   {
    LogPermaservClientErrors("Invalid name tag format in response from %s: %s "
                             "in HttpPermaservClient::getSingleValue.\n", fullUrl, recvBuf);
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
    LogPermaservClientErrors("Failed to obtain diffuse horizontal irradiance" 
                                                                    "from permaserv.\n");
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
  float retVal;
  if(getSingleValue((char*)"dni", (char*)"DNI", lat, longt, retVal))
    return retVal;
  else
   {
    LogPermaservClientErrors("Failed to obtain direct normal irradiance" 
                                                                    "from permaserv.\n");
    return 0.0f;
   }
}


// =======================================================================================
