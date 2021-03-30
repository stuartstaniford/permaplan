// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is to capture groups of trees

#ifndef COPSE_H
#define COPSE_H

#include "DisplayList.h"

// =======================================================================================
// Class variable initialization

class Copse: public DisplayList
{
public:
  
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




