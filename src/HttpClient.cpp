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
  curl_easy_setopt(easyHandle, CURLOPT_ERRORBUFFER, errorBuf);

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
// Helper C function to do the file writing for HttpClient::fetchFile via
// curl_easy_setopt(...CURLOPT_WRITEFUNCTION...)

size_t writeToFile(void *buffer, size_t size, size_t nmemb, void *userp)
{
  return fwrite(buffer, size, nmemb, (FILE*)userp);
}


// =======================================================================================
// Function which will fetch a file from a given URL and store it in a designated path
// location.  Returns success or failure.  
// XX Currently always starts over from the beginning if a prior transfer was incomplete.

bool HttpClient::fetchFile(const char* url, const char* path)
{
  FILE* file = fopen(path, "w");
  unless(file)
   {
    LogHttpClientErrors("Could not open file %s for writing.\n", path);
    return false;
   }
  
  curl_easy_setopt(easyHandle, CURLOPT_URL, url);
  curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, writeToFile);
  curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, file);

  CURLcode result = curl_easy_perform(easyHandle);
  if(result == CURLE_OK)
    return true;
  else
   {
    LogHttpClientErrors("Could not transfer from %s:%s.\n", url, errorBuf);
    return false;
   }
}


// =======================================================================================
