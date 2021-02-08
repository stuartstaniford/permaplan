// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for storing and working with the boundary of a plot of land, as in the OLDF
// boundary object.


#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <vector>
#include <cglm/cglm.h>
#include "HttpDebug.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

// =======================================================================================
// Class variable initialization


class Boundary
{
 public:
  
  // Instance variables - public
  vec2 referencePoint;

  // Member functions - public
  Boundary(void);
  ~Boundary();
  void setFromOLDF(rapidjson::Value& boundaries);
  bool validateReferencePoint(rapidjson::Value& boundaries);
  bool validateArcs(rapidjson::Value& boundaries);
  bool validateBoundaries(rapidjson::Value&  boundaries);
  bool diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  Boundary(const Boundary&);                              // Prevent copy-construction
  Boundary& operator=(const Boundary&);                   // Prevent assignment
};

#endif




