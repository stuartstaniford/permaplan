// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef HTTP_SERV_THREAD_H
#define HTTP_SERV_THREAD_H

#include "TaskQueue.h"
#include "HttpRequestParser.h"
#include "PermaservCookie.h"


// =======================================================================================
// Useful macros

#define httPrintf(...) if((serv->respPtr += snprintf(serv->respPtr, \
              serv->respEnd-serv->respPtr,  __VA_ARGS__)) >= serv->respEnd) \
                {serv->respBufOverflow = true; return false;}
#define internalPrintf(...) if((respPtr += snprintf(respPtr, \
              respEnd-respPtr,  __VA_ARGS__)) >= respEnd) \
                {respBufOverflow = true; return false;}

#define DEV_CACHE_DURATION 900
#define PROD_CACHE_DURATION 86400
#define CACHE_DURATION DEV_CACHE_DURATION


// =======================================================================================
// Forward declarations

class HttpLoadBalancer;
class UserRecord;
class UserManager;
class ClimateDatabase;
class UserSessionGroup;


// =======================================================================================
/// @brief Superclass for any thread that is going to handle the specifics of an HTTP
/// server.
///
/// This contains general functionality allowing it to be used by the system of 
/// HTTPLoadBalancer.  This shouldn't be instantiated directly, but rather should be 
/// subclassed, and the subclass should handle the specific requests of a given server
/// (eg see HttpDebug as the original subclass of this).

class HttpServThread: public TaskQueue
{
  friend HttpLoadBalancer;
  friend UserManager;
  friend ClimateDatabase;

public:
  
  // Instance variables - public
  char*               respPtr;
  char*               respEnd;
  bool                respBufOverflow;

protected:
  
  // Instance variables - protected
  HttpRequestParser   reqParser;
  PermaservCookie     cookies;
  unsigned            respBufSize;
  unsigned            headBufSize;
  char*               respBuf;
  char*               altResp; // used for static pages
  char*               altMimeType;
  char*               headBuf;
  unsigned            cacheDuration;                 
  unsigned short      clientP;
  HttpLoadBalancer*   parentLB;
  UserSessionGroup*   userSessions;
  UserRecord*         loggedInUser;
  
public:
  
  // Member functions - public
  HttpServThread(unsigned index, HttpLoadBalancer* parent, UserSessionGroup* userS = nullptr);
  ~HttpServThread(void);
  bool  startResponsePage(const char* title, unsigned refresh = 0u, 
                                                            char* scriptName = nullptr);
  bool  endResponsePage(void);
  bool  errorPage(const char* error);
  void  processOneHTTP1_1(int connfd, unsigned short clientPort);
  inline void setCacheDuration(int duration) 
   {
    cacheDuration = duration;
   }

  inline bool startTable(char* name = nullptr)
   {
    if(name)
     {
      internalPrintf("<table name=\"%s\">\n", name);
     }
    else
     {
      internalPrintf("<table>\n");
     }
    return true;
   }
  inline bool newSection(const char* title)
   {
    internalPrintf("<hr><center><h3>%s</h3>\n", title);
    return true;
   }
  inline void setAltResp(char* buf, unsigned size, char* mimeType)
   {
    altResp     = buf;
    respPtr     = buf + size;
    altMimeType = mimeType;
   }
  const char* getLoggedInUserName(void);

protected:
  
  // Member functions - protected
  virtual bool  processRequestHeader(void);


private:
  
  // Instance variables - private

  // Member functions - private
  bool          reallocateResponseBuf(void);
  void          dealWithPossibleCookies(void);
  unsigned      generateHeader(unsigned bodySize, unsigned code, const char* msg, 
                                                              char* mimeType = nullptr);
  bool          writeLoop(int fildes, char *buf, size_t nbyte);
  inline void   resetResponse(void)
   {
    respPtr         = respBuf;
    respEnd         = respBuf + respBufSize;
    altResp         = nullptr;
    altMimeType     = nullptr;
    loggedInUser    = nullptr;
    cacheDuration   = 0u;
   }  

  PreventAssignAndCopyConstructor(HttpServThread);       
};


// =======================================================================================

#endif




