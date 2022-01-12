// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// Superclass for any thread that is going to handle the specifics of an HTTP
// server.  This contains general functionality allowing it to be used by the system 
// of HTTPLoadBalancer.  This shouldn't be instantiated directly, but rather should be 
// subclassed, and the subclass should handle the specific requests of a given server
// (eg see HttpDebug as the original subclass of this).

#include "HttpServThread.h"


// =======================================================================================
/// @brief Constructor

HttpServThread::HttpServThread(unsigned index):
                                    TaskQueue(index),
                                    respBufOverflow(false),
                                    reqParser(8192),
                                    respBufSize(16384),
                                    headBufSize(4096)
{
  respBuf = new char[respBufSize];
  headBuf = new char[headBufSize];
  if(!respBuf || !headBuf)
    err(-1, "Couldn't allocate memory in __func__\n");
}


// =======================================================================================
/// @brief Destructor

HttpServThread::~HttpServThread(void)
{
}


// =======================================================================================
/// @brief Generate a response header into the header buffer.
///
/// Note headBufSize is currently 4096 so this won't stretch it.
/// @returns The number of bytes generated.
/// @param bodySize The size of the response body  
/// @param code The code number to generate on the HTTP status line (200, 404, etc)
/// @param msg A C-string of message on the status line ("OK", "ERROR", etc).

unsigned HttpServThread::generateHeader(unsigned bodySize, unsigned code, const char* msg)
{
  char* ptr = headBuf;
  if(strlen(msg) > headBufSize-1024)
    err(-1, "Excessive message size in HttpDebug::generateHeader.");
    
  ptr += sprintf(ptr, "HTTP/1.1 %u %s\r\n", code, msg);
  ptr += sprintf(ptr, "Content-Type: text/html\r\n");
  ptr += sprintf(ptr, "Content-Length: %u\r\n", bodySize);
  ptr += sprintf(ptr, "\r\n");
  return (ptr-headBuf);
}
  

// =======================================================================================
