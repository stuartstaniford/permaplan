// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef ORIENTABLE_H
#define ORIENTABLE_H

#include "Positionable.h"

// =======================================================================================
/// @brief Superclass for things that have both a position and are rotatable in the x-y
/// plane.
///
/// This class can be inherited from for things that are Positionable, but also can be
/// rotated around the z-axis (ie the up direction).  It provides some standard methods 
/// for adjusting the position and orientation (eg menu subpanels and on-screen arrows)

class Orientable: public Positionable
{
public:
  
  // Instance variables - public
  float angleFromNorth;
  
  // Member functions - public
  Orientable(void);
  ~Orientable(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  Orientable(const Orientable&);                 // Prevent copy-construction
  Orientable& operator=(const Orientable&);      // Prevent assignment

};

#endif




