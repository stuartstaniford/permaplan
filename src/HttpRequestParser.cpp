// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
// The purpose of this class is to read from a socket, identify the boundaries of
// whole HTTP request messages, and return a pointer to the beginning of such
// messages.  Right now, it doesn't deal with requests with entity bodies (because
// it doesn't need to).  It null terminates the header so that higher layers that
// call on this one can treat the request header as a single string if they wish.
// We try to minimize extra copying, but there may be situations where some copying
// within our buffer is necessary.

#include "HttpRequestParser.h"
#include "Global.h"
#include "Logging.h"
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <err.h>

// =======================================================================================
// Constructor

HttpRequestParser::HttpRequestParser(unsigned size):
                                          bufSize(size)
{
  buf  = new char[bufSize];
  unless(buf)
    err(-1, "Couldn't allocate memory in __func__\n");

}


// =======================================================================================
// Destructor

HttpRequestParser::~HttpRequestParser(void)
{
  delete[] buf;
}


// =======================================================================================
// Get the next header.  Currently returns pointer to the URL.

bool HttpRequestParser::getNextRequest(void)
{
  int nBytes;

  // Read some data
  if((nBytes = read(connfd, buf, bufSize)) <= 0)
   {
    // No data, so give up on this particular connection
    close(connfd);
    LogRequestErrors("Couldn't read data from socket.\n");
    return false;
   }
  //XX we need to handle this properly
  buf[nBytes] = '\0';
  LogHTTPDetails("Got from client: %s", buf);
  
  if(strncmp(buf, "GET ", 4) !=0 )
   {
    buf[20] = '\0';
    LogRequestErrors("Unsupported method in HTTP request %s.\n", buf);
    return false;
   }
  urlOffset = 4u;
  char* url = buf + urlOffset;
  char* lastToken = index(url, ' ');
  if(lastToken)
    *(lastToken++) = '\0';
  else
   {
    // This happens when we are rerunning a prior request after growing the buffer
    lastToken = url+strlen(url)+1;
   }
  if(lastToken - buf + 256 > bufSize)
   {
    LogRequestErrors("Header too large in HTTP request.\n");
    return false;
   }
  if( (strncmp(lastToken, "HTTP/1.1", 8) != 0) && (strncmp(lastToken, "HTTP/1.0", 8) != 0) )
   {
    LogRequestErrors("Unsupported HTTP version %s\n", lastToken);
    return false;
   }   
  return true;
}


// =======================================================================================
