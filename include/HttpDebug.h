// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// Super brain-dead HTTP server for debugging the running system.  Implements
// a very small subset of HTTP/1.1, but seems to work adequately for this system
// with all known browsers.  Only uses C lib calls to avoid complicating the
// build.


#ifndef HTTP_DEBUG_H
#define HTTP_DEBUG_H
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <err.h>


// =======================================================================================
// Class variable initialization

class Scene; // forward declaration

class HttpDebug
{
public:
  
  // Instance variables - public
  char*               respPtr;
  Scene&              scene;

  // Member functions - public
  HttpDebug(unsigned short servPort, Scene& S);
  ~HttpDebug(void);
  void* processConnections(void);
  void        startResponsePage(const char* title);
  void        endResponsePage(void);
  bool        errorPage(const char* error);
  inline void addResponseData(const char* data)
   {
    respPtr += sprintf(respPtr, "%s", data);
    if(respPtr - respBuf + 256 > respBufSize)
     err(-1, "Overflowing response buffer in __func__\n");
   }
  inline void startTable(void)
   {
    addResponseData("<table cellpadding=\"1\" border=\"1\">\n");
   }
  inline void newSection(const char* title)
   {
    respPtr += sprintf(respPtr, "<hr><center><h3>%s</h3>\n", title);
    if(respPtr - respBuf + 256 > respBufSize)
      err(-1, "Overflowing response buffer in __func__\n");
   }

private:
  
  // Instance variables - private
  unsigned            reqBufSize;
  unsigned            respBufSize;
  unsigned            headBufSize;
  char*               reqBuf;
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
  bool        indexPage(void);
  inline void resetResponse(void)
   {
    respPtr = respBuf;
   }
};

#endif





