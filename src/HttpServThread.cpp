// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// Superclass for any thread that is going to handle the specifics of an HTTP
// server.  This contains general functionality allowing it to be used by the system 
// of HTTPLoadBalancer.  This shouldn't be instantiated directly, but rather should be 
// subclassed, and the subclass should handle the specific requests of a given server
// (eg see HttpDebug as the original subclass of this).

#include "HttpServThread.h"
#include "Logging.h"

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
  delete[] respBuf;
  delete[] headBuf;
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
  

// ======================================================================================
/// @brief Grow the response buf when it's not big enough
/// @returns True if we successfully grew it, false if we ran up against the limit.

bool HttpServThread::reallocateResponseBuf(void)
{
  respBufSize *= 2;
  if(respBufSize > 16*1024*1024)
   {
    LogHTTPBufferOps("HTTP max response buffer exceeded at %u.\n", respBufSize);
    return false;
   }
  LogHTTPBufferOps("HTTP response buffer size increased to %u.\n", respBufSize);
  delete[] respBuf;
  respBuf = new char[respBufSize];
  respBufOverflow = false;
  resetResponse();
  return true;
}


// =======================================================================================
/// @brief Generate an HTML page opening into the response buffer.
/// @returns True if we successfully generated the HTML, false if we ran out of space.
/// @param title A C string to use in the title tag and also h1 tag at the top of the 
/// page.
/// @param refresh A refresh interval for the page (defaults to 0u if missing).

bool HttpServThread::startResponsePage(const char* title, unsigned refresh)
{
  internalPrintf("<!doctype html>\n<html lang=\"en\">\n<head>\n<meta charset=\"utf-8\">\n");
  internalPrintf("<title>%s</title>\n", title);
  internalPrintf("<link rel=\"stylesheet\" href=\"css/styles.css\">\n");
  if(refresh > 0)
    internalPrintf("<meta http-equiv=\"refresh\" content=\"%u\">\n", refresh);
  internalPrintf("</head>\n<body>\n");
  internalPrintf("<center><h1>%s</h1></center>\n", title);
  return true;
}


// =======================================================================================
/// @brief Generate an HTML page ending into the response buffer.
/// @returns True if we successfully generated the HTML, false if we ran out of space.

bool HttpServThread::endResponsePage(void)
{
  internalPrintf("<hr><center><span name=\"copyright\">Staniford Systems</span>");
  internalPrintf("</center>\n</body>\n</html>\n");
  return true;
}


// =======================================================================================
