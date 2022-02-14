// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class isolates HTTP client functionality from the rest of the code.  Currently is
// a gateway to libcurl.  Mainly used by the resource manager in fetching stuff.
// See the tutorial at https://curl.se/libcurl/c/libcurl-tutorial.html

#include "HttpClient.h"
#include "Logging.h"
#include "Global.h"
#include <err.h>
#include <string.h>


// =======================================================================================
// Static variable initialization

bool HttpClient::globalInitCalled = false;


// =======================================================================================
/// @brief Constructor.  Get's a Curl easy handle.

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
/// @brief Destructor.  Cleans up the Curl handle.
/// @todo curl_global_cleanup is not presently called.

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
// Helper C function to do the file writing for HttpClient::fetchBuffer via
// curl_easy_setopt(...CURLOPT_WRITEFUNCTION...)

size_t writeToBuffer(void *buffer, size_t size, size_t nmemb, void *userp)
{
  SizedBuffer* S = (SizedBuffer*)userp;
  
  if(S->size == 0u)
    return 0u;
  
  if(size*nmemb < S->size)
   {
    memcpy(S->buf, buffer, size*nmemb);
    S->size -= size*nmemb;
    return size*nmemb;
   }
  else
   {
    LogHttpClientErrors("Overflow in writeToBuffer, %lu cannot fit in %u.\n", size*nmemb,
                        S->size);
    memcpy(S->buf, buffer, S->size-1);
    S->buf[S->size-1] = '\0';
    S->size = 0u;
    return S->size-1;
   }
}


// =======================================================================================
/// @brief  Fetch a file from a given URL and store it in a designated path location.  
/// @returns true on success or false on failure.
/// @param url A const C-string with the URL to fetch from
/// @param path A C-string with the path location where the file is to be stored (we
/// will open this file and then close it at the end). 
/// @todo Currently always starts over from the beginning if a prior transfer was incomplete.

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
  fclose(file);
  if(result == CURLE_OK)
    return true;
  else
   {
    LogHttpClientErrors("Could not transfer from %s:%s.\n", url, errorBuf);
    return false;
   }
}


// =======================================================================================
/// @brief  Fetch a response from a given URL and store it in a designated memory buffer.  
/// @returns true on success or false on failure.
/// @param url A const C-string with the URL to fetch from
/// @param buf A char* buffer to write the data to.
/// @param bufSize An unsigned denoting the size of the buffer in bytes. 
/// @todo Currently always starts over from the beginning if a prior transfer was incomplete.

bool HttpClient::fetchBuffer(const char* url, char* buf, unsigned bufSize)
{
  SizedBuffer S;
  S.size  = bufSize;
  S.buf   = buf;
  
  curl_easy_setopt(easyHandle, CURLOPT_URL, url);
  curl_easy_setopt(easyHandle, CURLOPT_WRITEFUNCTION, writeToBuffer);
  curl_easy_setopt(easyHandle, CURLOPT_WRITEDATA, &S);

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
