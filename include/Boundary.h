// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -

#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <vector>
#include <cglm/cglm.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

// =======================================================================================
// Forward declarations

class HttpDebug;
class BoundingBox;


// =======================================================================================
/// @brief Class for storing and working with the boundary of a plot of land, as in 
/// the OLDF boundary object.

class Boundary
{
 public:
  
  // Instance variables - public
  vec2 referencePoint;       // lat, long - see OLDF doc for details
  std::vector<float> arcs;   // in spaceUnits - see OLDF doc

  // Member functions - public
  Boundary(void);
  ~Boundary();
  void setFromOLDF(rapidjson::Value& boundaries);
  bool extendBoundingBox(BoundingBox& box);
  bool validateReferencePoint(rapidjson::Value& boundaries);
  bool validateArcs(rapidjson::Value& boundaries);
  bool validateBoundaries(rapidjson::Value&  boundaries);
  void writeOLDFSection(FILE* file, char* indent);
  bool diagnosticHTML(HttpDebug* serv);
  void latLongRange(float& loLat, float& hiLat, float& loLong, float& hiLong);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  Boundary(const Boundary&);                              // Prevent copy-construction
  Boundary& operator=(const Boundary&);                   // Prevent assignment
};


// =======================================================================================

#endif




