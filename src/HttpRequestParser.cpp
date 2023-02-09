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
/// @brief Constructor
/// @param size The number of bytes to use in the request parsing buffer (currently we 
/// actually allocate 8 more than this to have some guard bytes at either end).

HttpRequestParser::HttpRequestParser(unsigned size):
                                          bufSize(size)
{
  buf  = new char[size+8];
  unless(buf)
    err(-1, "Couldn't allocate memory in __func__\n");
  strncpy(buf, "aaaa", 4);
  buf+=4;
  strncpy(buf+size, "bbbb", 4);

  // most initialization in here: HTTP/1.1 reinitializes multiple times
  resetForReuse();  
  
  LogRequestParsing("Request parser initialized with buffer size %u.\n", bufSize);
  headerMap["Connection"]         = Connection;
  headerMap["User-Agent"]         = UserAgent;
  headerMap["Content-Length"]     = ContentLength;
  headerMap["Content-Type"]       = ContentType;
  headerMap["Transfer-Encoding"]  = TransferEncoding;
  headerMap["Upgrade"]            = Upgrade;
}


// =======================================================================================
/// @brief Reset the parser for a new connection.
/// 
/// Function to reset (eg for a new connection) without throwing away the buffer.  
/// Needs to duplicate the newly constructed state.

void HttpRequestParser::resetForReuse(void)
{
  readPoint           = NULL;
  headerEnd           = NULL;
  urlOffset           = 0u;
  httpVerOffset       = 0u;
  connectionDone      = false;
  connectionWillClose = false;
  requestMethod       = NoMethod;
  bodyPresent         = false;
  bodySize            = 0u;
}


// =======================================================================================
/// @brief Destructor

HttpRequestParser::~HttpRequestParser(void)
{
  buf-=4;
  delete[] buf;
}


// =======================================================================================
/// @brief Parses the request once we know we have a whole header in the buffer
/// @returns True if we were able to parse the request to our satisfaction, false 
/// otherwise.

bool HttpRequestParser::parseRequest(void)
{
  char* h, *url, *lastToken;
  
  // Deal with method
  if(strncmp(buf, "GET ", 4) == 0)
   {
    requestMethod = GET;
    urlOffset = 4u;
   }
  else if(strncmp(buf, "POST ", 5) == 0)
   {
    requestMethod = POST;
    urlOffset = 5u;
   }
  else
   {
    buf[20] = '\0';
    LogRequestErrors("Unsupported method in HTTP request %s.\n", buf);
    goto badParseRequestExit;
   }
  
  // Deal with URL
  url = buf + urlOffset;
  lastToken = index(url, ' ');
  if(lastToken)
    *(lastToken++) = '\0';
  else
   {
    // This happens when we are rerunning a prior request after growing the buffer
    lastToken = url+strlen(url)+1;
   }
  LogRequestParsing("Found Url of length %lu: %s.\n", strlen(url), url);
  
  // Deal with HTTP version
  if(lastToken - buf + 256 > bufSize)
   {
    LogRequestErrors("Header too large in HTTP request.\n");
    goto badParseRequestExit;
   }
  if( (strncmp(lastToken, "HTTP/1.1", 8) != 0) && (strncmp(lastToken, "HTTP/1.0", 8) != 0) )
   {
    LogRequestErrors("Unsupported HTTP version %s\n", lastToken);
    goto badParseRequestExit;
   }
  
  // Parse rest of the header lines
  h = lastToken + 10;
  while(h < headerEnd)
   {
    char* term = index(h, ':');
    unless(term && term < headerEnd-1)
      break;
    *term = '\0';
    char* value = term + 1;
    while(isspace(*value))
      value++;
    term = index(value, '\r');
    unless(term && term < headerEnd-3)
      break;
    *term = '\0';
    if(headerMap.count(h)) // A header we recognize
     {
      switch(headerMap[h])
       {
          case Connection:
            LogRequestParsing("Found Connection header %s.\n", value);
            if(strcmp(value, "close") == 0)
              connectionWillClose = true;
            break;
          
          case UserAgent:
            LogRequestParsing("Found User-Agent header %s.\n", value);
            break;
          
          case ContentLength:
            LogRequestParsing("Found Content-Length header (%s) in HTTP request.\n", value);
            bodyPresent = true;
            bodySize = atoi(value);
            break;

          case ContentType:
            LogRequestErrors("Unsupported Content-Type header in HTTP request.\n");
            goto badParseRequestExit;

          case TransferEncoding:
            LogRequestErrors("Unsupported Transfer-Encoding header in HTTP request.\n");
            goto badParseRequestExit;

          case Upgrade:
            LogRequestErrors("Unsupported Upgrade header  in HTTP request.\n");
            goto badParseRequestExit;
          
          default:
            LogRequestParsing("Header with no special handling: %s.\n", value);
       }
     }
    h = term + 2;
   }
    
  // Finish up in successful case
  headerEnd = NULL; // make sure we don't reuse a crazy value next time.
  return true;
  
  // Cleanup if we had to abort
badParseRequestExit:
  connectionDone = true;
  LogFlush();
  return false;
}
   

// =======================================================================================
/// @brief State machine to check a range of bytes looking for \r\n\r\n.  
/// 
/// @returns If present, a pointer to one past the final \n.  Otherwise, returns NULL.  
/// @param range A char* pointer to the beginning of the range of bytes to check.
/// @param rangeSize The number of bytes in the range to check.

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
       {
        LogRequestParsing("Found header end %lu bytes into range\n", p-range);
        return p+1;
       }
      else if(*p == '\r')
        state = 1;
      else
        state = 0;
     }
   }
  return NULL;
}


// =======================================================================================
/// @brief Get the next request by reading from the socket.
/// @returns true if we successfully read a request, false otherwise.

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
    LogRequestParsing("Moving %u bytes from position %lu in buffer\n", nBytes, readPoint-buf);
    memcpy(buf, readPoint, nBytes);
    readPoint = buf + bufLeft;
    bufLeft = bufSize - bufLeft;
    leftOverDataPresent = true;
   }
  else
   { 
    LogRequestParsing("getNextRequest with no leftover data.\n");
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
        connectionDone = true;
        return false;
       }
      if(nBytes < bufLeft)
       {
        // Got some data, read it into buffer
        readPoint[nBytes] = '\0';
        LogRequestParsing("Read %u bytes into position %lu in buffer\n", nBytes, readPoint-buf);
       }
      else // damn big request
       {
        LogRequestErrors("Request too big for buffer.\n");
        connectionDone = true;
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
    if((headerEnd = headerEndPresent(checkStart, checkSize)))
     {
      // This is good, we get to go home, but first we keep track of unused data
      if(headerEnd - readPoint < nBytes)
       {
        bufLeft = nBytes - (headerEnd - readPoint);
        readPoint = headerEnd;
        LogRequestParsing("Leftover %u bytes at position %lu in buffer\n", bufLeft, readPoint-buf);
       }
      else
        readPoint = NULL;
      break;
     }
      
    // About to go round again, so update the paramters for where/how much to read
    readPoint += nBytes;
    bufLeft -= nBytes;
  
   } // while(1) over reads
  
  unless(parseRequest())
   {
    readPoint = NULL;
    return false;
   }
  
  if(bodyPresent)
    return processBody();

  readPoint = NULL;
  return true;
}


// =======================================================================================
/// @brief Determine if there's a body, and read it in if so.
/// @returns true if we successfully read any body, or there is no body, false otherwise.

bool HttpRequestParser::processBody(void)
{

  readPoint = NULL;
  return true;  
}


// =======================================================================================
