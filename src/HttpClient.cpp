// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class isolates HTTP client functionality from the rest of the code.  Currently is
// a gateway to libcurl.  Mainly used by the resource manager in fetching stuff.

#include "HttpClient.h"
#include <curl/curl.h>


// =======================================================================================
// Constructor

HttpClient::HttpClient(void)
{
  curl_global_init(CURL_GLOBAL_DEFAULT);
}


// =======================================================================================
// Destructor

HttpClient::~HttpClient(void)
{
  curl_global_cleanup();
}


// =======================================================================================
