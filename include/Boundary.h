// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for storing and working with the boundary of a plot of land, as in the OLDF
// boundary object.


#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <vector>
#include <cglm/cglm.h>
#include "HttpDebug.h"

// =======================================================================================
// Class variable initialization


class Boundary
{
 public:
  
  // Instance variables - public

  // Member functions - public
  Boundary(void);
  ~Boundary();
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  Boundary(const Boundary&);                              // Prevent copy-construction
  Boundary& operator=(const Boundary&);                   // Prevent assignment
};

#endif




