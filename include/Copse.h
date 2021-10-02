// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -

#ifndef COPSE_H
#define COPSE_H

#include "DisplayList.h"


// =======================================================================================
// Forward declarations

class BoundingBox;


// =======================================================================================
/// @brief This class is to capture groups of trees.  
/// 
/// It's used especially in growth simulation to form copses that are processed together 
/// for shade-tracking purposes.
/// @todo This is only a stub class right now, but is still believed to be necessary 
/// as of Sep 2021.

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


// =======================================================================================

#endif




