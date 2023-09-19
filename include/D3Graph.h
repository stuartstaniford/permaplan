// Copyright Staniford Systems.  All Rights Reserved.  September 2023 -

#ifndef D3_GRAPH_H
#define D3_GRAPH_H

#include "Global.h"


// =======================================================================================
// Forward declarations

class HttpServThread;


// =======================================================================================
/// @brief Provide support for making graphs via D3 in web pages served via 
/// HttpServThread and it's subclasses.
///
/// https://d3js.org/what-is-d3
/// https://www.freecodecamp.org/news/d3js-tutorial-data-visualization-for-beginners/.

class D3Graph
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  D3Graph(void);
  ~D3Graph(void);

  // Static public functions  
  static bool includeD3Script(HttpServThread* serv);
  static bool startChart(HttpServThread* serv, int pixWidth, int pixHeight);
  static bool endChart(HttpServThread* serv);

private:

  // Static private functions  
  static bool openSVG(HttpServThread* serv, int pixWidth, int pixHeight);
  static bool closeSVG(HttpServThread* serv);
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(D3Graph);
};


// =======================================================================================

#endif




