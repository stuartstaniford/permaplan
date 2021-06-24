// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class isolates HTTP client functionality from the rest of the code.  Currently is
// a gateway to libcurl.  Mainly used by the resource manager in fetching stuff.
// See the tutorial at https://curl.se/libcurl/c/libcurl-tutorial.html

#include "HttpClient.h"
#include "Logging.h"


// =======================================================================================
// Static variable initialization

bool HttpClient::globalInitCalled = false;


// =======================================================================================
// Constructor

HttpClient::HttpClient(void)
{
  unless(globalInitCalled)
   {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    globalInitCalled = true;
   }
  
  easyHandle = curl_easy_init();
  unless(easyHandle)
    err(-1, "Cannot get handle from curl_easy_init.\n");
  
}


// =======================================================================================
// Destructor

HttpClient::~HttpClient(void)
{
  curl_easy_cleanup(easyHandle);
  
  //XX would need some book-keeping to keep track of the last one closed
  //curl_global_cleanup();
}


// =======================================================================================
// Function which will fetch a file from a given URL and store it in a designated path
// location.  Returns success or failure.  
// XX Currently always starts over from the beginning if a prior transfer was incomplete.

bool HttpClient::fetchFile(char* url, char* path)
{
  return true;
}


// =======================================================================================
