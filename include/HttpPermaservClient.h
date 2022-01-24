// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef HTTP_PERMASERV_CLIENT_H
#define HTTP_PERMASERV_CLIENT_H

#include "HttpClient.h"


// =======================================================================================
/// @brief Access permaserv from within permaplan
///
/// This implements the client side of the permaserv api, and also implements caching
/// of answers from permaserv so we don't bug it too often.

class HttpPermaservClient: public HttpClient
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpPermaservClient(void);
  ~HttpPermaservClient(void);
  bool getSingleValue(char* url, char* name, float lat, float longt, float& retVal);
  float getDIFValue(float lat, float longt);
  float getDNIValue(float lat, float longt);

  // Static public member functions
  static HttpPermaservClient& getPermaservClient(void) // Get the singleton instance
   {
    return *theClient;
   }

private:
  
  // Private static variables
  static HttpPermaservClient* theClient;

  // Instance variables - private
  unsigned short servPort;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpPermaservClient(const HttpPermaservClient&);       
  /// @brief Prevent assignment.
  HttpPermaservClient& operator=(const HttpPermaservClient&);      
};


// =======================================================================================

#endif




