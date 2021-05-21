// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// Super brain-dead HTTP server for debugging the running system.  Implements
// a very small subset of HTTP/1.1, but seems to work adequately for this system
// with all known browsers.  Only uses C lib calls to avoid complicating the
// build.


#ifndef HTTP_DEBUG_H
#define HTTP_DEBUG_H

#include "HttpRequestParser.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <err.h>
#include <cstdio>


// =======================================================================================
// Useful macros

#define httPrintf(...) if((serv->respPtr += snprintf(serv->respPtr, \
              serv->respEnd-serv->respPtr,  __VA_ARGS__)) >= serv->respEnd) \
                {serv->respBufOverflow = true; return false;}
#define internalPrintf(...) if((respPtr += snprintf(respPtr, \
              respEnd-respPtr,  __VA_ARGS__)) >= respEnd) \
                {respBufOverflow = true; return false;}

// =======================================================================================
// Class variable initialization

// forward declarations
class Scene; 
class MenuInterface; 

class HttpDebug
{
public:
  
  // Instance variables - public
  char*               respPtr;
  char*               respEnd;
  Scene&              scene;
  MenuInterface&      menuInterface;
  bool                shutDownNow; // Reads/writes on a single bool should be atomic, no lock.
  bool                respBufOverflow;

  // Member functions - public
  HttpDebug(unsigned short servPort, Scene& S, MenuInterface& imgMenu);
  ~HttpDebug(void);
  void*       processConnections(void);
  bool        startResponsePage(const char* title, unsigned refresh = 0u);
  bool        endResponsePage(void);
  bool        errorPage(const char* error);
  inline bool startTable(void)
   {
    internalPrintf("<table cellpadding=\"1\" border=\"1\">\n");
    return true;
   }
  inline bool newSection(const char* title)
   {
    internalPrintf("<hr><center><h3>%s</h3>\n", title);
    return true;
   }

private:
  
  // Instance variables - private
  HttpRequestParser   reqParser;
  unsigned            respBufSize;
  unsigned            headBufSize;
  char*               respBuf;
  char*               headBuf;
  struct sockaddr_in  servaddr;
  struct sockaddr_in  cliaddr;
  int                 sockfd;
  int                 connfd;
  unsigned short      port;

  // Member functions - private
  void        processOneHTTP1_1(void);
  unsigned    generateHeader(unsigned bodySize, unsigned code, const char* msg);
  bool        processRequestHeader(void);
  bool        reallocateResponseBuf(void);
  bool        indexPage(void);
  HttpDebug(const HttpDebug&);                 // Prevent copy-construction
  HttpDebug& operator=(const HttpDebug&);      // Prevent assignment

  inline void resetResponse(void)
   {
    respPtr = respBuf;
    respEnd = respBuf + respBufSize;
   }
};

#endif





