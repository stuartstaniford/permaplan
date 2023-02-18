// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef HTTP_SERV_THREAD_H
#define HTTP_SERV_THREAD_H

#include "TaskQueue.h"
#include "HttpRequestParser.h"
#include "UserManager.h"
#include "ClimateDatabase.h"


// =======================================================================================
// Useful macros

#define httPrintf(...) if((serv->respPtr += snprintf(serv->respPtr, \
              serv->respEnd-serv->respPtr,  __VA_ARGS__)) >= serv->respEnd) \
                {serv->respBufOverflow = true; return false;}
#define internalPrintf(...) if((respPtr += snprintf(respPtr, \
              respEnd-respPtr,  __VA_ARGS__)) >= respEnd) \
                {respBufOverflow = true; return false;}


// =======================================================================================
// Forward declarations

class HttpLoadBalancer;


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
  friend bool UserManager::processHttpRequest(HttpServThread* serv, char* url);
  friend bool ClimateDatabase::processClimateRequest(HttpServThread* serv, char* url, 
                                                                          bool diagnostic);

public:
  
  // Instance variables - public
  char*               respPtr;
  char*               respEnd;
  bool                respBufOverflow;

protected:
  
  // Instance variables - protected
  HttpRequestParser   reqParser;
  unsigned            respBufSize;
  unsigned            headBufSize;
  char*               respBuf;
  char*               altResp; // used for static pages
  char*               altMimeType;
  char*               headBuf;
  unsigned short      clientP;
  HttpLoadBalancer*   parentLB;
  
  
public:
  
  // Member functions - public
  HttpServThread(unsigned index, HttpLoadBalancer* parent);
  ~HttpServThread(void);
  bool  startResponsePage(const char* title, unsigned refresh = 0u);
  bool  endResponsePage(void);
  bool  errorPage(const char* error);
  void  processOneHTTP1_1(int connfd, unsigned short clientPort);
  inline bool startTable(char* name = NULL)
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

protected:
  
  // Member functions - protected
  unsigned      generateHeader(unsigned bodySize, unsigned code, const char* msg, 
                                                              char* mimeType = NULL);
  bool          reallocateResponseBuf(void);
  bool          writeLoop(int fildes, char *buf, size_t nbyte);
  virtual bool  processRequestHeader(void);

  inline void resetResponse(void)
   {
    respPtr     = respBuf;
    respEnd     = respBuf + respBufSize;
    altResp     = NULL;
    altMimeType = NULL;
   }

private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpServThread(const HttpServThread&);       
  /// @brief Prevent assignment.
  HttpServThread& operator=(const HttpServThread&);      
};


// =======================================================================================

#endif




