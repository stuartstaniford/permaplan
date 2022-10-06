// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef HTTP_PERMASERV_CLIENT_H
#define HTTP_PERMASERV_CLIENT_H

#include "HttpClient.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include <vector>

#define JSON_OBJ_BUFSIZE 16384


// =======================================================================================
// Forward declarations

class SoilProfile;
class SoilDatabaseClient;
class ClimateDatabaseClient;


// =======================================================================================
/// @brief Access permaserv from within permaplan
///
/// This implements the client side of the permaserv api, and also implements caching
/// of answers from permaserv so we don't bug it too often.

class HttpPermaservClient: public HttpClient
{
  friend SoilDatabaseClient;
  friend ClimateDatabaseClient;
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpPermaservClient(void);
  ~HttpPermaservClient(void);
  void writeCacheFile(void);
  bool getSingleValue(char* url, char* name, float lat, float longt, float& retVal);
  bool getJSONObject(char* url);
  float getDIFValue(float lat, float longt);
  float getDNIValue(float lat, float longt);
  bool getSoilProfiles(float lowLat, float lowLong, float hiLat, float hiLong);
  bool getClimateData(float lat, float longT, unsigned years);

  // Static public member functions
  static HttpPermaservClient& getPermaservClient(void) // Get the singleton instance
   {
    return *theClient;
   }

private:
  
  // Private static variables
  static HttpPermaservClient* theClient;

  // Instance variables - private
  unsigned short        servPort;
  bool                  cachePresent;
  char                  recvBuf[JSON_OBJ_BUFSIZE];
  
  // Actual values that are cached and/or fetched.
  float                 dif;
  float                 dni;
  rapidjson::Document   doc;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpPermaservClient(const HttpPermaservClient&);       
  /// @brief Prevent assignment.
  HttpPermaservClient& operator=(const HttpPermaservClient&);      
};


// =======================================================================================

#endif




