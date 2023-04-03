// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// Superclass for any thread that is going to handle the specifics of an HTTP
// server.  This contains general functionality allowing it to be used by the system 
// of HTTPLoadBalancer.  This shouldn't be instantiated directly, but rather should be 
// subclassed, and the subclass should handle the specific requests of a given server
// (eg see HttpDebug as the original subclass of this).

#include "HttpServThread.h"
#include "UserSession.h"
#include "UserManager.h"
#include "Logging.h"
#include <unistd.h>


// =======================================================================================
/// @brief Constructor

HttpServThread::HttpServThread(unsigned index, HttpLoadBalancer* parent, 
                                                                UserSessionGroup* userS):
                                    TaskQueue(index),
                                    respBufOverflow(false),
                                    reqParser(8192),
                                    respBufSize(16384),
                                    headBufSize(4096),
                                    clientP(0),
                                    parentLB(parent),
                                    userSessions(userS)
{
  respBuf = new char[respBufSize];
  headBuf = new char[headBufSize];
  if(!respBuf || !headBuf)
    err(-1, "Couldn't allocate memory in __func__\n");
  resetResponse();
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
/// @param mimeType A C-string of the mime type for the content.  If nullptr (the default)
/// then "text/html" will be used.

unsigned HttpServThread::generateHeader(unsigned bodySize, unsigned code, 
                                        const char* msg, char* mimeType)
{
  char* ptr = headBuf;
  if(strlen(msg) > headBufSize-1024)
    err(-1, "Excessive message size in HttpDebug::generateHeader.");
    
  ptr += sprintf(ptr, "HTTP/1.1 %u %s\r\n", code, msg);
  if(mimeType)
    ptr += sprintf(ptr, "Content-Type: %s\r\n", mimeType);
  else
    ptr += sprintf(ptr, "Content-Type: text/html\r\n");
  ptr += cookies.sprint(ptr);
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
  internalPrintf("<!doctype html>\n<html lang=\"en\">\n<head>\n"
                                                        "<meta charset=\"utf-8\">\n");
  internalPrintf("<title>%s</title>\n", title);
  internalPrintf("<link rel=\"stylesheet\" type=\"text/css\" "
                                                  "href=\"/css/permaplan.css\">\n");
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
/// @brief Generate an Error Page.
/// @param error A C string with the specific error to tell the user.

bool HttpServThread::errorPage(const char* error)
{
  unless(startResponsePage("Error"))
    return false;
  internalPrintf("Sorry, an error has occurred: <b>");
  internalPrintf("%s", error);
  internalPrintf(".</b>\n");
  unless(endResponsePage())
    return false;
  return true;
}


// =======================================================================================
/// @brief Utility function to keep writing till we've gotten it done, or encounter error
/// @returns true if success, false if there's a write failure.
/// @param fildes The file descriptor of the socket to write to.
/// @param buf The char buffer to write from
/// @param nbyte The number of bytes that must be written.

bool HttpServThread::writeLoop(int fildes, char *buf, size_t nbyte)
{
  char* p = buf;
  int bytesWritten;
  
  while(p-buf < nbyte)
   {
    bytesWritten = write(fildes, p, nbyte - (p-buf));
    if(bytesWritten < 0)
     {
      LogRequestErrors("Write call failed in writeLoop\n");
      return false;
     }
    p += bytesWritten;
   }
  return true;
}


// =======================================================================================
/// @brief Interface for method to process a single header, and construct the response.
/// 
/// This is the main interface that a subclass must implement in order to examine and
/// act on a given request (eg by interrogating the reqParser protected variable).
/// The implementation in this class should never be called, and errs out if it is. 

bool HttpServThread::processRequestHeader(void)
{
   err(-1, "Unimplemented superclass version of HttpServThread::processRequestHeader.\n");
}


// =======================================================================================
/// @brief Function to break out request Cookie handling from processOneHTTP1_1.
/// 

void HttpServThread::dealWithPossibleCookies(void)
{
  char* cookieVal = reqParser.getCookieString();
  if(cookieVal)
     cookies.processRequestCookies(cookieVal);
  if(cookies.flags & VALID_SESSION_ID)
   {
    EntryStatus status;
    UserManager& userMan = UserManager::getUserManager();
    loggedInUser = userMan.getRecord(cookies.sessionId, status, userSessions);
    if(loggedInUser)
     {
      LogPermaservOpDetails("Session ID for this request is now %llX on user %s.\n", 
                                      cookies.sessionId, loggedInUser->userName.c_str());
     }
    else
     {
      LogPermaservOpDetails("Session ID %llX in request is not valid.\n", cookies.sessionId);
     }
   }
}


// =======================================================================================
/// @brief Process one HTTP/1.1 connection, looping over the request headers and 
/// arranging for them to be dealt with individually.
/// @param connfd The socket file descriptor
/// @param clientPort The client port of the underlying TCP connection (mainly for 
/// logging purposes).

void HttpServThread::processOneHTTP1_1(int connfd, unsigned short clientPort)
{
  LogHTTPLoadBalance("HTTPDebug %d handling client on port %u.\n", queueIndex, clientPort);
  clientP = clientPort; // make this available for logging by subclasses.
  
  reqParser.setNewConnection(connfd);

  while(reqParser.getNextRequest())
   {
    resetResponse();
    dealWithPossibleCookies();
        
    // Process the rest of the request (and generate a response body)
    unsigned headerLen;
    bool returnOK;
    while(1)
     {
      returnOK = processRequestHeader();
      LogHTTPLoadBalance("HTTPDebug %d: client port %u, request %s.\n", 
                          queueIndex, clientPort, reqParser.getUrl());
      if(returnOK)
        break;
      unless(respBufOverflow)
       {
        LogHTTPBufferOps("None size related false return from processRequestHeader.\n");
        break;
       }
      unless(reallocateResponseBuf())
        break;
     }
    
    // Generate the correct response header
    if(returnOK)
     {
      if(altResp)
       {
        headerLen = generateHeader(respPtr-altResp, 200, "OK", altMimeType);
       }
      else
       {
        headerLen = generateHeader(respPtr-respBuf, 200, "OK");
       }
     }
    else
     {
      LogRequestErrors("500 error being returned on HTTP request.\n");
      headerLen = generateHeader(0u, 500, "ERROR");
     }
    
    // Respond to the client client
    unless(writeLoop(connfd, headBuf, headerLen))
      break;
    if(returnOK)
     {
      if(altResp)
       {
        unless(writeLoop(connfd, altResp, respPtr-altResp))
          break;        
       }
      else
       {
        unless(writeLoop(connfd, respBuf, respPtr-respBuf))
          break;
       }
     }
    //fprintf(stderr, "timeToDie on %d is %d.\n", queueIndex, timeToDie);
    if(timeToDie || reqParser.connectionWillClose)
     {
      reqParser.resetForNewConnection();
      break;      
     }
    else
      reqParser.resetForNewRequest();      
   }
}


// =======================================================================================
