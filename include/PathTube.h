// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Class for rendering an element that consists of a tube (of varying width) that
// follows a specified path (set of points).  Provides support to things like 
// semicircular arrows.

#ifndef PATH_TUBE_H
#define PATH_TUBE_H

#include "VisualElement.h"

// =======================================================================================
// Class variable initialization

class PathTube: public VisualElement
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  PathTube(void);
  ~PathTube(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  PathTube(const PathTube&);                 // Prevent copy-construction
  PathTube& operator=(const PathTube&);      // Prevent assignment

};

#endif




