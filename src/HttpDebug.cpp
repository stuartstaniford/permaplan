// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// Super brain-dead HTTP server for debugging the running system.  Implements
// a very small subset of HTTP/1.1, but seems to work adequately for this system
// with all known browsers.  Only uses C lib calls to avoid complicating the
// build.

#include "HttpDebug.h"
#include "Scene.h"
#include "Tree.h"
#include "MemoryTracker.h"
#include "TaskQueueFarm.h"
#include "SkySampleModel.h"
#include "InterfaceAction.h"
#include "InterfaceMainSceneWin.h"
#include "MainSceneWindow.h"
#include "Window3D.h"
#include "Species.h"
#include "GLFWApplication.h"
#include <stdio.h>


// =======================================================================================
/// @brief Constructor
/// @param S A reference to the Scene object
/// windowApp A reference to the application object that manages the windows
/// index An unsigned index of which TaskQueue we are in the loadBalancer.

HttpDebug::HttpDebug(Scene& S, GLFWApplication& winApp, unsigned index):
                        HttpServThread(index),
                        scene(S),
                        menuInterface(NULL),
                        windowApp(winApp)
{
  MainSceneWindow& mainWin = (MainSceneWindow&)winApp.getMainWin();
  menuInterface = mainWin.imgMenu;
  resetResponse();
}


// ======================================================================================
/// @brief Destructor deallocates the buffers.

HttpDebug::~HttpDebug(void)
{
}


// =======================================================================================
/// @brief Generate the index page.
/// 
/// This generates the root navigation page for the whole debugging HTTP interface.
/// @returns True if we successfully generated the HTML, false if we ran out of space
/// (which should never happen...)

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
  internalPrintf("<td>Camera variables for current window</td></tr>\n");
  
  // Scene indicator tbuf
  internalPrintf("<tr><td><a href=\"/stbuf/\">stbuf/</a></td>");
  internalPrintf("<td>Scene indicator triangle buffer</td></tr>\n");

  // Scene object tbuf
  internalPrintf("<tr><td><a href=\"/otbuf/\">otbuf/</a></td>");
  internalPrintf("<td>Scene object triangle buffer</td></tr>\n");

  // Memory Usage Tracking
  internalPrintf("<tr><td><a href=\"/memtrack/\">memtrack/</a></td>");
  internalPrintf("<td>Application memory usage tracking</td></tr>\n");

  // Taskqueues
  internalPrintf("<tr><td><a href=\"/taskqueues/\">taskqueues/</a></td>");
  internalPrintf("<td>Task Queue status</td></tr>\n");

  // Sky Samples
  internalPrintf("<tr><td><a href=\"/skysamples/\">skysamples/</a></td>");
  internalPrintf("<td>Current sky sampling vectors</td></tr>\n");

  // Windows
  internalPrintf("<tr><td><a href=\"/window/list/\">window/list/</a></td>");
  internalPrintf("<td>Currently open windows on screen</td></tr>\n");

  // End table and page
  internalPrintf("</table></center>\n");
  scene.diagnosticHTMLSimulationSummary(this);

  endResponsePage();
  return true;
}


// =======================================================================================
/// @brief Generate the page that summarizes carbon capture
/// @returns True if we successfully generated the HTML, false if we ran out of space

bool HttpDebug::carbonSummary(void)
{
  unless(startResponsePage("Carbon Capture Summary"))
    return false;
  
  // Beginning of table
  internalPrintf("<center>\n");
  unless(startTable((char*)"carbonCapture"))
    return false;
  internalPrintf("<tr><th>Row</th><th>Object-Id</th><th>Entity</th>"
                                          "<th>Carbon Captured (tonnes)</th></tr>\n");
  

  // End table and page
  internalPrintf("</table></center>\n");
  endResponsePage();
  return true;
}


// =======================================================================================
/// @brief Process a single header, and construct the response.
/// 
/// Calls the HTTRequestParser instance to extract the URL, and then routes the request
/// to whatever the apppropriate other object is to handle it.
/// @returns True if all went well, false if we couldn't correctly write a good page.

bool HttpDebug::processRequestHeader(void)
{
  char* url = reqParser.getUrl();
  
  if( (strlen(url) == 1 && url[0] == '/') || strncmp(url, "/index.", 7) == 0)
    return indexPage();

  scene.lock();
  bool retVal = false;

  // Possible paths (in alphabetical order  
  if( strlen(url) == 7 && strncmp(url, "/alive/", 7) == 0)
   {
    internalPrintf("OK\n");
    retVal = true;
   }
  
  else if( strlen(url) >= 8 && strncmp(url, "/camera/", 8) == 0)
   {
    Window3D& win = windowApp.getActiveWin();
    retVal =  win.camera.diagnosticHTML(this, url+8, scene);
   }
  
  else if( strlen(url) == 8 && strncmp(url, "/carbon/", 8) == 0)
    retVal =  carbonSummary();

  else if( strlen(url) >= 7 && strncmp(url, "/click/", 7) == 0)
   {
    InterfaceAction* action = new InterfaceAction(Click, url+7);
    if(action->valid)
     {
      scene.actions.push_back(action);
      internalPrintf("OK\n");
      retVal = true;
     }
    else
      LogRequestErrors("Couldn't create valid click action from %s\n", url+7);
  }

  else if( strlen(url) >= 13 && strncmp(url, "/doubleclick/", 13) == 0)
   {
    InterfaceAction* action = new InterfaceAction(DoubleClick, url+13);
    if(action->valid)
     {
      scene.actions.push_back(action);
      internalPrintf("OK\n");
      retVal = true;
     }
    else
      LogRequestErrors("Couldn't create valid double click action from %s\n", url+13);
   }
    
  else if( strlen(url) == 6 && strncmp(url, "/land/", 6) == 0)
    retVal =  scene.land.diagnosticHTML(this);

  else if( strlen(url) >= 8 && strncmp(url, "/logset/", 8) == 0)
    retVal =  LogControlHTML(this, url+8);

  else if( strlen(url) == 10 && strncmp(url, "/memtrack/", 10) == 0)
    retVal =  MemoryTracker::diagnosticHTML(this);

  else if( strlen(url) >=6 && strncmp(url, "/menu/", 6) == 0)
    retVal =  menuInterface->HTTPAPi(this, url+6);

  else if( strlen(url) > 8 && strncmp(url, "/object/", 8) == 0)
    retVal =  VisualObject::diagnosticHTMLSelection(this, url+8);

  else if( strlen(url) == 7 && strncmp(url, "/otbuf/", 7) == 0)
    retVal =  scene.sceneObjectTbuf->diagnosticHTML(this);

  else if( strlen(url) >= 8 && strncmp(url, "/plants/", 8) == 0)
    retVal =  Tree::treePageGateway(this, url+8);

  else if( strlen(url) >= 6 && strncmp(url, "/quad/", 6) == 0)
    retVal = scene.qtree->diagnosticHTML(this, url+6);

  else if( strlen(url) > 12 && strncmp(url, "/quadsearch/", 12) == 0)
    retVal = scene.qtree->quadSearchHTML(this, url+12);

  else if( strlen(url) == 6 && strncmp(url, "/quit/", 6) == 0)
   {
    InterfaceAction* action = new InterfaceAction(QuitProgram, url+6);
    if(action->valid)
     {
      scene.actions.push_back(action);
      retVal = true;
     }
    else
      LogRequestErrors("Couldn't create valid QuitProgram action from %s\n", url+7);
   }

  else if( strlen(url) == 12 && strncmp(url, "/skysamples/", 12) == 0)
   {
    SkySampleModel& sky = SkySampleModel::getSkySampleModel();
    retVal =  sky.diagnosticHTML(this);
   }
  
  else if( strlen(url) > 14 && strncmp(url, "/species/", 9) == 0)
    retVal =  Species::findSpeciesForHTTPDebug(this, url+9);

  else if( strlen(url) == 7 && strncmp(url, "/stbuf/", 7) == 0)
    retVal =  scene.indicatorTbuf->diagnosticHTML(this);  

  else if( strlen(url) == 12 && strncmp(url, "/taskqueues/", 12) == 0)
    retVal =  threadFarm->diagnosticHTML(this);

  else if( strlen(url) > 8 && strncmp(url, "/window/", 8) == 0)
    retVal =  windowApp.HTTPGateway(this, url+8);
  
  else
   {
    LogRequestErrors("Request for unknown resource %s\n", url);
    errorPage("Resource not found");
   }
  
  scene.unlock();

  return retVal;
}


// =======================================================================================
/// @brief Process one HTTP/1.1 connection, looping over the request headers and 
/// arranging for them to be dealt with individually.
/// @param connfd The socket file descriptor
/// @param clientPort The client port of the underlying TCP connection (mainly for 
/// logging purposes).

void HttpDebug::processOneHTTP1_1(int connfd, unsigned short clientPort)
{
  LogHTTPLoadBalance("HTTPDebug %d handling client on port %u.\n", queueIndex, clientPort);

  reqParser.setNewConnection(connfd);
  
  while(reqParser.getNextRequest())
   {
    resetResponse();

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
    if(reqParser.connectionWillClose)
        break;
   }
  reqParser.resetForReuse();
}


// =======================================================================================
