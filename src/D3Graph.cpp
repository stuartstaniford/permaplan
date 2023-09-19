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
