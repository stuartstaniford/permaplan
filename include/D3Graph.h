// Copyright Staniford Systems.  All Rights Reserved.  September 2023 -

#ifndef D3_GRAPH_H
#define D3_GRAPH_H

#include "Global.h"


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
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(D3Graph);
};


// =======================================================================================

#endif




