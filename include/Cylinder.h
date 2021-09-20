// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -

#ifndef CYLINDER_H
#define CYLINDER_H

#include "AxialElement.h"


// =======================================================================================
/// @brief Class for rendering a cylinder.
///
/// Almost all the real implementation is inherited from AxialElement.

class Cylinder: public AxialElement
{
 public:
  
  // Instance variables - public

  // Member functions - public
  Cylinder(vec3 root, vec3 dir, float R, unsigned sides);
  ~Cylinder(void);
  int           printOPSF(char*& buf, unsigned bufSize);
  const char*   objectName(void);
  bool          diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  vec2          cylinderPoints[2];
  
  // Member functions - private
  Cylinder(const Cylinder&);                 // Prevent copy-construction
  Cylinder& operator=(const Cylinder&);      // Prevent assignment
};


// =======================================================================================

#endif




