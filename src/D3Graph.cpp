// Copyright Staniford Systems.  All Rights Reserved.  September 2023 -
// Provide support for making graphs via D3 in web pages served via 
// HttpServThread and it's subclasses.
//
// https://d3js.org/what-is-d3
// https://www.freecodecamp.org/news/d3js-tutorial-data-visualization-for-beginners/.

#include "D3Graph.h"
#include "HttpServThread.h"


// =======================================================================================
/// @brief Constructor

D3Graph::D3Graph(void)
{
}


// =======================================================================================
/// @brief Destructor

D3Graph::~D3Graph(void)
{
}


// =======================================================================================
/// @brief Output a script tag to include the D3 library.
/// 
/// This is where we select which version of D3 is currently in use in Permaplan/Permaserv
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool D3Graph::includeD3Script(HttpServThread* serv)
 {
  httPrintf("<script src=\"https://d3js.org/d3.v7.min.js\" "
                                        "charset=\"utf-8\"></script>\n"); 
  return true;
 }


// =======================================================================================
/// @brief Output a locally sourced script tag.
/// 
/// This is to include a script served from the local scripts directory on the current 
/// server (ie by permaserv/permaplan).  Note that only scripts set up in 
/// HttpLBPermaserv::initializeScriptPages will work.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param scriptName The path of the script below /scripts/

bool D3Graph::insertLocalScriptTag(HttpServThread* serv, char* scriptName)
 {
  httPrintf("<script src=\"/scripts/%s\" charset=\"utf-8\"></script>\n", scriptName); 
  return true;
 }


// =======================================================================================
/// @brief Start a new D3 chart in an HTML page.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param pixWidth The number of pixels wide to make the chart area.
/// @param pixHeight The number of pixels high to make the chart area.
 
bool D3Graph::startChart(HttpServThread* serv, int pixWidth, int pixHeight)
 {
  openSVG(serv, pixWidth, pixHeight);
  return true;
 }


// =======================================================================================
/// @brief Open an SVG area in an HTML page.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.
/// @param pixWidth The number of pixels wide to make the svg area.
/// @param pixHeight The number of pixels high to make the svg area.

bool D3Graph::openSVG(HttpServThread* serv, int pixWidth, int pixHeight)
 {
  httPrintf("<svg width=\"%d\" height=\"%d\">\n",  pixWidth, pixHeight); 
  return true;
 }


// =======================================================================================
/// @brief End a D3 chart in an HTML page.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool D3Graph::endChart(HttpServThread* serv)
 {
  closeSVG(serv);
  return true;
 }


// =======================================================================================
/// @brief Close an SVG area in an HTML page.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool D3Graph::closeSVG(HttpServThread* serv)
 {
  httPrintf("</svg>\n"); 
  return true;
 }

// =======================================================================================
