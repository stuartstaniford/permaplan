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
  inline Positionable(void) {};
  inline Positionable(float x, float y, float z) 
    {position[0] = x; position[1] = y; position[2] = z;}
  inline Positionable(vec3 P) 
    {position[0] = P[0]; position[1] = P[1]; position[2] = P[2];}

  ~Positionable(void);
  
protected:
  
  // Instance variables - protected

private:
  
  // Member functions - private
  Positionable(const Positionable&);                 // Prevent copy-construction
  Positionable& operator=(const Positionable&);      // Prevent assignment

};

#endif




