// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is to capture groups of trees.  It's used especially in growth simulation
// to form copses that are processed together for shade-tracking purposes.

#ifndef COPSE_H
#define COPSE_H

#include "DisplayList.h"

// =======================================================================================
// Class variable initialization

class Copse: public DisplayList
{
public:
  
  BoundingBox* box;
  
  // Instance variables - public
  
  // Member functions - public
  Copse(void);
  ~Copse(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  Copse(const Copse&);                 // Prevent copy-construction
  Copse& operator=(const Copse&);      // Prevent assignment

};

#endif




