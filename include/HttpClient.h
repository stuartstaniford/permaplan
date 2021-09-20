// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <curl/curl.h>


// =======================================================================================
/// @brief This class isolates HTTP client functionality from the rest of the code.  
/// 
/// Currently it is a gateway to libcurl.  Mainly used by the resource manager 
/// in fetching stuff. // See the tutorial at 
/// https://curl.se/libcurl/c/libcurl-tutorial.html


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
  char          errorBuf[CURL_ERROR_SIZE];
  
  // Member functions - private
  HttpClient(const HttpClient&);                 // Prevent copy-construction
  HttpClient& operator=(const HttpClient&);      // Prevent assignment

};

// =======================================================================================

#endif




