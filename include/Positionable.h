// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef POSITIONABLE_H
#define POSITIONABLE_H

#include <cglm/cglm.h>


// =======================================================================================
/// @brief Superclass for things that have a position.
///
/// This class can be inherited from for things that have a position and provides some
/// standard methods for adjusting the position (eg menu subpanels and on-screen arrows.
/// In this class, the [0] (x) and [1] (y) directions have the obvious absolute meaning,
/// but the [2] (z) direction is interpreted relative to the ground surface.

class Positionable
{
public:
  
  // Instance variables - public
  vec3 position;
  
  // Member functions - public
  Positionable(void);
  ~Positionable(void);
  
protected:
  
  // Instance variables - protected

private:
  
  // Member functions - private
  Positionable(const Positionable&);                 // Prevent copy-construction
  Positionable& operator=(const Positionable&);      // Prevent assignment

};

#endif




