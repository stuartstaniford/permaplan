// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class isolates HTTP client functionality from the rest of the code.  Currently is
// a gateway to libcurl.  Mainly used by the resource manager in fetching stuff.

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <curl/curl.h>

// =======================================================================================
// Class variable initialization

class HttpClient
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpClient(void);
  ~HttpClient(void);
  bool fetchFile(const char* url, const char* path);
  
private:
  
  // Instance variables - private
  static bool   globalInitCalled;
  CURL*         easyHandle;
  
  // Member functions - private
  HttpClient(const HttpClient&);                 // Prevent copy-construction
  HttpClient& operator=(const HttpClient&);      // Prevent assignment

};

#endif




