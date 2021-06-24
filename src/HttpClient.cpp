// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class isolates HTTP client functionality from the rest of the code.  Currently is
// a gateway to libcurl.  Mainly used by the resource manager in fetching stuff.

#include "HttpClient.h"
#include "Logging.h"

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
