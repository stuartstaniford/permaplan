// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// Claff for rendering an arrow that follows part of the arc of a circle.

#ifndef SEMI_CIRCULAR_ARROW_H
#define SEMI_CIRCULAR_ARROW_H

#include "PathTube.h"

// =======================================================================================
// Class variable initialization

class SemicircularArrow: public PathTube
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  SemicircularArrow(void);
  ~SemicircularArrow(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  SemicircularArrow(const SemicircularArrow&);                 // Prevent copy-construction
  SemicircularArrow& operator=(const SemicircularArrow&);      // Prevent assignment

};

#endif




