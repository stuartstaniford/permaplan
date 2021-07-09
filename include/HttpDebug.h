// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -

#ifndef HTTP_DEBUG_H
#define HTTP_DEBUG_H

#include "HttpRequestParser.h"
#include "TaskQueue.h"
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
// forward declarations

class Scene; 
class MenuInterface; 


// =======================================================================================
/// Simple internal HTTP server for debugging the running system.  
/// 
/// Implements a very small subset of HTTP/1.1, but seems to work adequately for this system
/// with all known browsers.  Only uses C lib calls to avoid complicating the
/// build.

class HttpDebug: public TaskQueue
{  
public:
  
  // Instance variables - public
  char*               respPtr;
  char*               respEnd;
  Scene&              scene;
  MenuInterface&      menuInterface;
  bool                respBufOverflow;

  // Member functions - public
  HttpDebug(Scene& S, MenuInterface& imgMenu, unsigned index);
  ~HttpDebug(void);
  bool        startResponsePage(const char* title, unsigned refresh = 0u);
  bool        endResponsePage(void);
  bool        errorPage(const char* error);
  bool        carbonSummary(void);
  void        processOneHTTP1_1(int connfd, unsigned short clientPort);
  inline bool startTable(char* name = NULL)
   {
    if(name)
     {
      internalPrintf("<table name=\"%s\" cellpadding=\"1\" border=\"1\">\n", name);
     }
    else
     {
      internalPrintf("<table cellpadding=\"1\" border=\"1\">\n");
     }
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

  // Member functions - private
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





