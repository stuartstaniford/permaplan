// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <curl/curl.h>


// =======================================================================================
/// @brief POD Helper class for passing a sized buffer to a callback function

class SizedBuffer
{
public:
  
  char* buf;
  unsigned size;
};


// =======================================================================================
/// @brief This class isolates HTTP client functionality from the rest of the code.  
/// 
/// Used by the resource manager in fetching stuff, and also used when permaplan 
/// wants to query permaserv for things (HttpPermaservClient calls this).
/// 
/// Currently it is a gateway to libcurl.   See the tutorial at 
/// https://curl.se/libcurl/c/libcurl-tutorial.html

class HttpClient
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpClient(void);
  ~HttpClient(void);
  bool fetchFile(const char* url, const char* path);
  bool fetchBuffer(const char* url, char* buf, unsigned bufSize);
  
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




