// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for use in rendering parts of buildings - specifically it renders a
// rectangular box with a gable roof above it.  Multiple of these can be superimposed
// to create approximations of many buildings.  Support for texturing is provided.
// A gable is implemented as two sheds

#ifndef GABLE_H
#define GABLE_H

#include "Shed.h"


// =======================================================================================
// Class variable initialization

class Gable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Gable(void);
  ~Gable(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  Gable(const Gable&);                 // Prevent copy-construction
  Gable& operator=(const Gable&);      // Prevent assignment

};

#endif




