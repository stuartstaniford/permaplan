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
#define SA struct sockaddr


// =======================================================================================
// Create the socket and bind to the port in the constructor

HttpDebug::HttpDebug(unsigned short servPort, Scene& S):
                        scene(S),
                        shutDownNow(false),
                        reqBufSize(8192),
                        respBufSize(16384),
                        port(servPort)
{
  reqBuf  = new char[reqBufSize];
  respBuf = new char[respBufSize];
  headBuf = new char[headBufSize];
  if(!reqBuf || !respBuf || !headBuf)
    err(-1, "Couldn't allocate memory in __func__\n");
  respPtr = respBuf;

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
  startResponsePage("App Debugging Interface");
  
  // Beginning of table
  addResponseData("<center>\n");
  startTable();
  addResponseData("<tr><th>Link</th><th>notes</th></tr>\n");
  
  // Quadtree
  addResponseData("<tr><td><a href=\"/quad/\">quad/</a></td>");
  addResponseData("<td>Examine the quadtree</td></tr>\n");

  // Land Surface
  addResponseData("<tr><td><a href=\"/land/\">land/</a></td>");
  addResponseData("<td>Information about the land surface model.</td></tr>\n");

  // Plants
  addResponseData("<tr><td><a href=\"/plants/\">plants/</a></td>");
  addResponseData("<td>Table of all trees and other plants</td></tr>\n");

  // Camera
  addResponseData("<tr><td><a href=\"/camera/\">camera/</a></td>");
  addResponseData("<td>Current camera variables</td></tr>\n");
  
  // Scene indicator tbuf
  addResponseData("<tr><td><a href=\"/stbuf/\">tbuf/</a></td>");
  addResponseData("<td>Scene indicator triangle buffer</td></tr>\n");

  // Scene object tbuf
  addResponseData("<tr><td><a href=\"/otbuf/\">tbuf/</a></td>");
  addResponseData("<td>Scene object triangle buffer</td></tr>\n");

  // End table and page
  addResponseData("</table></center>\n");
  endResponsePage();
  return true;
}


// =======================================================================================
// Generate an Error Page

bool HttpDebug::errorPage(const char* error)
{
  startResponsePage("Error");
  addResponseData("Sorry, an error has occurred: <b>");
  addResponseData(error);
  addResponseData(".</b>\n");
  endResponsePage();
  return true;
}


// =======================================================================================
// Process a single header, and construct the response

bool HttpDebug::processRequestHeader(void)
{
  if(strncmp(reqBuf, "GET ", 4) !=0 )
   {
    errorPage("Unsupported method in HTTP request");
    return false;
   }
  char* url = reqBuf + 4;
  char* lastToken = index(url, ' ');
  *(lastToken++) = '\0';
  if(lastToken - reqBuf + 256 > reqBufSize)
   {
    errorPage("Bad header format");
    return false;
   }
  if( (strncmp(lastToken, "HTTP/1.1", 8) != 0) && (strncmp(lastToken, "HTTP/1.0", 8) != 0) )
   {
    errorPage("Unsupported HTTP version");
    return false;
   }
     
  // Start processing the url
  if( (strlen(url) == 1 && url[0] == '/') || strncmp(url, "/index.", 7) == 0)
    return indexPage();
  
  if( strlen(url) >= 6 && strncmp(url, "/quad/", 6) == 0)
    return scene.qtree->diagnosticHTML(this, url+6);

  if( strlen(url) == 7 && strncmp(url, "/stbuf/", 7) == 0)
    return scene.indicatorTbuf->diagnosticHTML(this);
  
  if( strlen(url) == 7 && strncmp(url, "/otbuf/", 7) == 0)
    return scene.sceneObjectTbuf->diagnosticHTML(this);

  if( strlen(url) == 6 && strncmp(url, "/land/", 6) == 0)
    return scene.land.diagnosticHTML(this);

  if( strlen(url) == 8 && strncmp(url, "/plants/", 8) == 0)
    return Tree::allTreeDiagnosticHTML(this);

  if( strlen(url) ==8 && strncmp(url, "/camera/", 8) == 0)
    return scene.camera.diagnosticHTML(this);

  errorPage("Resource not found");
  return false;
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
      return;
     }
    //XX we need to handle this properly
    reqBuf[nBytes] = '\0';
    
    printf("Got from client: %s\n", reqBuf);
    resetResponse();

    unsigned headerLen;
    if(processRequestHeader())
      headerLen = generateHeader(respPtr-respBuf, 200, "OK");
    else
      headerLen = generateHeader(respPtr-respBuf, 404, "NOT FOUND");
    
    // respond to client
    write(connfd, headBuf, headerLen);
    write(connfd, respBuf, respPtr-respBuf);

    //XX need to check errors on write
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

void HttpDebug::startResponsePage(const char* title)
{
  respPtr += sprintf(respPtr, "<!doctype html>\n<html lang=\"en\">\n<head>\n<meta charset=\"utf-8\">\n");
  respPtr += sprintf(respPtr, "<title>%s</title>\n", title);
  respPtr += sprintf(respPtr, "<link rel=\"stylesheet\" href=\"css/styles.css\">\n");
  respPtr += sprintf(respPtr, "</head>\n<body>\n");
  respPtr += sprintf(respPtr, "<center><h1>%s</h1></center>\n", title);
  if(respPtr - respBuf + 256 > respBufSize)
    err(-1, "Overflowing response buffer in __func__\n");
}


// =======================================================================================
// Generate an HTML page ending into the response buffer.

void HttpDebug::endResponsePage(void)
{
  respPtr += sprintf(respPtr, "</body>\n</html>\n");
  if(respPtr - respBuf + 256 > respBufSize)
    err(-1, "Overflowing response buffer in __func__\n");
}

// =======================================================================================
