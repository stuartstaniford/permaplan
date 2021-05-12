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
                                          readPoint(NULL),
                                          bufSize(size),
                                          urlOffset(0u),
                                          httpVerOffset(0u)
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
// This parses the header once we know we have a whole header in the buffer

bool HttpRequestParser::parseRequest(void)
{
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
// State machine to check a range of bytes looking for \r\n\r\n.  If present, returns
// a pointer to the final \n.  Otherwise, returns NULL.  

char* HttpRequestParser::headerEndPresent(char* range, unsigned rangeSize)
{
  int state = 0;
  char* end = range + rangeSize;
  
  for(char* p = range; p < end; p++)
   {
    if(state == 0)
     {
      if(*p == '\r')
        state++;
      continue;
     }
    if(state == 1)
     {
      if(*p == '\n')
        state++;
      else if(*p != '\r')
        state = 0;
      continue;
     }
    if(state == 2)
     {
      if(*p == '\r')
        state++;
      else
        state = 0;
      continue;
     }
    if(state == 3)
     {
      if(*p == '\n')
        return p;
      else if(*p == '\r')
        state = 1;
      else
        state = 0;
     }
   }
  return NULL;
}


// =======================================================================================
// Get the next header.  Currently returns pointer to the URL.

bool HttpRequestParser::getNextRequest(void)
{ 
  bool  leftOverDataPresent;
  int   nBytes;
  
  if(readPoint)
   {
    // There is leftover data from a prior call.
    //XX note if performance became an issue, we could have a bigger buffer and
    //XX more complex book-keeping to reduce these copies.
    nBytes = bufLeft;
    memcpy(buf, readPoint, nBytes);
    readPoint = buf + bufLeft;
    bufLeft = bufSize - bufLeft;
    leftOverDataPresent = true;
   }
  else
   { 
    readPoint = buf;
    bufLeft = bufSize;
    leftOverDataPresent = false;
   }
  
  while(1) // until we've read enough data for a complete header
   {    
    if(leftOverDataPresent)
      leftOverDataPresent = false;
    else 
     {
      // Read some data
      if((nBytes = read(connfd, readPoint, bufLeft)) <= 0)
       {
        // No data, so give up on this particular connection
        close(connfd);
        LogRequestErrors("Couldn't read data from socket.\n");
        return false;
       }
      if(nBytes < bufLeft)
        readPoint[nBytes] = '\0';
      else // damn big request
       {
        LogRequestErrors("Request too big for buffer.\n");
        return false;
       }
      LogHTTPDetails("Got from client: %s", readPoint);
     
     } // Reading if there's no leftover data
     
    // Now check to see if there's a header-end present in this latest read
     
    char* checkStart = readPoint-3; // in case of \r\n\r\n across last read boundary
    unsigned checkSize = nBytes + 3;
    if(checkStart < buf)
     {
      checkSize -= buf - checkStart;
      checkStart = buf;
     }
    char* headerEnd;
    if((headerEnd = headerEndPresent(checkStart, checkSize)))
     {
      // This is good, we get to go home, but first we keep track of unused data
      if(headerEnd - readPoint + 1 < nBytes)
       {
        bufLeft = nBytes - (headerEnd - readPoint + 1);
        readPoint = headerEnd+1;
       }
      else
        readPoint = NULL;
      break;
     }
      
    // About to go round again, so update the paramters for where/how much to read
    readPoint += nBytes;
    bufLeft -= nBytes;
  
   } // while(1) over reads
  
  return parseRequest();
}


// =======================================================================================
