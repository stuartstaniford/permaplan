// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -

#ifndef COLOREDAXES_H
#define COLOREDAXES_H

#include "LineStripList.h"


// =======================================================================================
/// @brief This class just draws X, Y, and Z axes in red, green, and blue fixed colors.

class ColoredAxes: public LineStripList
{
 public:
  
  // Member functions - public
  ColoredAxes(float axesLen);
  ~ColoredAxes(void);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  ColoredAxes(const ColoredAxes&);                 // Prevent copy-construction
  ColoredAxes& operator=(const ColoredAxes&);      // Prevent assignment
};

#endif



