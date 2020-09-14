// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// This class just draws X, Y, and Z axes in red, green, and blue fixed colors


#ifndef COLOREDAXES_H
#define COLOREDAXES_H

#include "LineStripList.h"

// =======================================================================================
// Class variable initialization


class ColoredAxes: public LineStripList
{
 public:
  
  
  // Member functions - public
  ColoredAxes(Shader& S, float axesLen);
  ~ColoredAxes(void);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  

};

#endif



