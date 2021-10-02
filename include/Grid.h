// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -

#ifndef GRID_H
#define GRID_H

#include "LineStripList.h"
#include <cglm/cglm.h>


// =======================================================================================
// Forward declarations

class LandSurface;


// =======================================================================================
/// @brief This class draws a square grid at a designated scale over the land surface.

class Grid: public LineStripList
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Grid(LandSurface& L, float gridSpacing, float alt = 0.0f);
  void newHeight(float z);
  ~Grid(void);
  
private:
  
  // Instance variables - private
  float                 spacing;
  LandSurface&          land;
  int                   nX;
  int                   nY;
  unsigned              NHeights;
  float                 sumHeights;
  
  // Member functions - private
  void resetAltitude(float alt);
  Grid(const Grid&);                 // Prevent copy-construction
  Grid& operator=(const Grid&);      // Prevent assignment
};


// =======================================================================================

#endif




