// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// Super brain-dead HTTP server for debugging the running system.  Implements
// a very small subset of HTTP/1.1, but seems to work adequately for this system
// with all known browsers.  Only uses C lib calls to avoid complicating the
// build.

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "HttpDebug.h"
#include "Scene.h"
#include "Tree.h"
#include "MemoryTracker.h"
#include "TaskQueueFarm.h"

#define SA struct sockaddr


// =======================================================================================
// Create the socket and bind to the port in the constructor

HttpDebug::HttpDebug(unsigned short servPort, Scene& S):
                        scene(S),
                        shutDownNow(false),
                        respBufOverflow(false),
                        reqBufSize(8192),
                        respBufSize(16384),
                        port(servPort)
{
  reqBuf  = new char[reqBufSize];
  respBuf = new char[respBufSize];
  headBuf = new char[headBufSize];
  if(!reqBuf || !respBuf || !headBuf)
    err(-1, "Couldn't allocate memory in __func__\n");
  resetResponse();

  // Get a socket
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err(-1, "Couldn't create socket in __func__\n");

  // Set up our address and port
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  // Bind the socket to the port
  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
    err(-1, "Couldn't bind socket on port %u in __func__\n", port);

  // Make our socket a server that will listen
  if ((listen(sockfd, 6)) != 0)
    err(-1, "Listen failed on socket %d in __func__\n", sockfd);
}


// ======================================================================================
// Destructor deallocates the buffers and closes the socket

HttpDebug::~HttpDebug(void)
{
  close(sockfd);
  delete[] reqBuf;
  delete[] respBuf;
  delete[] headBuf;
}


// ======================================================================================
// For when the response buf is not big enough

bool HttpDebug::reallocateResponseBuf(void)
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
// Loop sitting on the socket accepting connections, process requests and reply to them.
// Arguments and return type are set up to be used in pthread_create

void* HttpDebug::processConnections(void)
{
  while(1)
   {
    // Accept a connection
    unsigned len = sizeof(cliaddr);
    if((connfd = accept(sockfd, (SA*)&cliaddr, &len)) < 0)
     {
      if(shutDownNow)
        break;
      else
        err(-1, "Accept failed on socket %d in __func__\n", sockfd);
     }
    processOneHTTP1_1();
   }
  
  LogCloseDown("HttpDebug server shutting down normally.\n");
  return NULL;
}


// =======================================================================================
// Generate the index page

bool HttpDebug::indexPage(void)
{
  unless(startResponsePage("App Debugging Interface"))
    return false;
  
  // Beginning of table
  internalPrintf("<center>\n");
  unless(startTable())
    return false;
  internalPrintf("<tr><th>Link</th><th>notes</th></tr>\n");
  
  // Quadtree
  internalPrintf("<tr><td><a href=\"/quad/\">quad/</a></td>");
  internalPrintf("<td>Examine the quadtree</td></tr>\n");

  // Land Surface
  internalPrintf("<tr><td><a href=\"/land/\">land/</a></td>");
  internalPrintf("<td>Information about the land surface model.</td></tr>\n");

  // Plants
  internalPrintf("<tr><td><a href=\"/plants/\">plants/</a></td>");
  internalPrintf("<td>Table of all trees and other plants</td></tr>\n");

  // Camera
  internalPrintf("<tr><td><a href=\"/camera/\">camera/</a></td>");
  internalPrintf("<td>Current camera variables</td></tr>\n");
  
  // Scene indicator tbuf
  internalPrintf("<tr><td><a href=\"/stbuf/\">tbuf/</a></td>");
  internalPrintf("<td>Scene indicator triangle buffer</td></tr>\n");

  // Scene object tbuf
  internalPrintf("<tr><td><a href=\"/otbuf/\">tbuf/</a></td>");
  internalPrintf("<td>Scene object triangle buffer</td></tr>\n");

  // Memory Usage Tracking
  internalPrintf("<tr><td><a href=\"/memtrack/\">memtrack/</a></td>");
  internalPrintf("<td>Application memory usage tracking</td></tr>\n");

  // Taskqueues
  internalPrintf("<tr><td><a href=\"/taskqueues/\">taskqueues/</a></td>");
  internalPrintf("<td>Task Queue status</td></tr>\n");

  // End table and page
  internalPrintf("</table></center>\n");
  endResponsePage();
  return true;
}


// =======================================================================================
// Generate an Error Page

bool HttpDebug::errorPage(const char* error)
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
// Process a single header, and construct the response

bool HttpDebug::processRequestHeader(void)
{
  if(strncmp(reqBuf, "GET ", 4) !=0 )
   {
    errorPage("Unsupported method in HTTP request");
    LogRequestErrors("Unsupported method in HTTP request.\n");
    return false;
   }
  char* url = reqBuf + 4;
  char* lastToken = index(url, ' ');
  if(lastToken)
    *(lastToken++) = '\0';
  else
   {
    // This happens when we are rerunning a prior request after growing the buffer
    lastToken = url+strlen(url)+1;
   }
  if(lastToken - reqBuf + 256 > reqBufSize)
   {
    errorPage("Bad header format");
    LogRequestErrors("Header too large in HTTP request.\n");
    return false;
   }
  if( (strncmp(lastToken, "HTTP/1.1", 8) != 0) && (strncmp(lastToken, "HTTP/1.0", 8) != 0) )
   {
    LogRequestErrors("Unsupported HTTP version %s\n", lastToken);
    errorPage("Unsupported HTTP version");
    return false;
   }
     
  // Start processing the url
  if( (strlen(url) == 1 && url[0] == '/') || strncmp(url, "/index.", 7) == 0)
    return indexPage();

  scene.lock();
  bool retVal = false;
  
  if( strlen(url) >= 6 && strncmp(url, "/quad/", 6) == 0)
    retVal = scene.qtree->diagnosticHTML(this, url+6);

  else if( strlen(url) == 7 && strncmp(url, "/stbuf/", 7) == 0)
    retVal =  scene.indicatorTbuf->diagnosticHTML(this);
  
  else if( strlen(url) == 7 && strncmp(url, "/otbuf/", 7) == 0)
    retVal =  scene.sceneObjectTbuf->diagnosticHTML(this);

  else if( strlen(url) == 6 && strncmp(url, "/land/", 6) == 0)
    retVal =  scene.land.diagnosticHTML(this);

  else if( strlen(url) >= 8 && strncmp(url, "/plants/", 8) == 0)
    retVal =  Tree::treePageGateway(this, url+8);

  else if( strlen(url) > 14 && strncmp(url, "/species/", 9) == 0)
    retVal =  Species::findSpeciesForHTTPDebug(this, url+9);

  else if( strlen(url) == 10 && strncmp(url, "/memtrack/", 10) == 0)
    retVal =  MemoryTracker::diagnosticHTML(this);

  else if( strlen(url) == 8 && strncmp(url, "/camera/", 8) == 0)
    retVal =  scene.camera.diagnosticHTML(this);

  else if( strlen(url) == 12 && strncmp(url, "/taskqueues/", 12) == 0)
    retVal =  threadFarm->diagnosticHTML(this);

  else
   {
    LogRequestErrors("Request for unknown resource %s\n", url);
    errorPage("Resource not found");
   }
  
  scene.unlock();

  return retVal;
}


// =======================================================================================
// Utility function to keep writing till we've gotten it done, or encounter error

bool writeLoop(int fildes, char *buf, size_t nbyte)
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
// Read HTTP 1.1 headers and extract the URL for processing.  We are brain-dead and only
// can deal with GET requests with no entity body.

void HttpDebug::processOneHTTP1_1()
{
  while(1)
   {
    int nBytes;

    // Read some data
    if((nBytes = read(connfd, reqBuf, reqBufSize)) <= 0)
     {
      // No data, so give up on this particular connection
      close(connfd);
      LogRequestErrors("Couldn't read data from socket.\n");
      return;
     }
    //XX we need to handle this properly
    reqBuf[nBytes] = '\0';
    
    LogHTTPDetails("Got from client: %s", reqBuf);
    resetResponse();

    unsigned headerLen;
    bool returnOK;
    while(1)
     {
      returnOK = processRequestHeader();
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
    if(returnOK)
      headerLen = generateHeader(respPtr-respBuf, 200, "OK");
    else
     {
      LogRequestErrors("500 error being returned on HTTP request.\n");
      headerLen = generateHeader(0u, 500, "ERROR");
     }
    
    // respond to client
    unless(writeLoop(connfd, headBuf, headerLen))
      break;
    if(returnOK)
      unless(writeLoop(connfd, respBuf, respPtr-respBuf))
        break;
  }
}


// =======================================================================================
// Generate a response header into the header buffer.

unsigned HttpDebug::generateHeader(unsigned bodySize, unsigned code, const char* msg)
{
  char* ptr = headBuf;
  ptr += sprintf(ptr, "HTTP/1.1 %u %s\r\n", code, msg);
  ptr += sprintf(ptr, "Content-Type: text/html\r\n");
  ptr += sprintf(ptr, "Content-Length: %u\r\n", bodySize);
  ptr += sprintf(ptr, "\r\n");
  return (ptr-headBuf);
}
  

// =======================================================================================
// Generate an HTML page opening into the response buffer.

bool HttpDebug::startResponsePage(const char* title, unsigned refresh)
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
// Generate an HTML page ending into the response buffer.

bool HttpDebug::endResponsePage(void)
{
  internalPrintf("</body>\n</html>\n");
  return true;
}

// =======================================================================================
