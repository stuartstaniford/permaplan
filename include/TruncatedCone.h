// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -

#ifndef TRUNCATED_CONE_H
#define TRUNCATED_CONE_H

#include "AxialElement.h"


// =======================================================================================
/// @brief Render a section of a cone cut off at right angles at both ends.
/// This is used for rendering sections of tree-trunks and the like.  Most of the 
/// implementation is inherited from AxialElement.

class TruncatedCone: public AxialElement
{
 public:
  
  // Instance variables - public
  float     smallRadius;

  // Member functions - public
  TruncatedCone(vec3 root, vec3 dir, float r, float R, unsigned S);
  ~TruncatedCone(void);
  int           printOPSF(char*& buf, unsigned bufSize);
  const char*   objectName(void);
  bool          diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  vec2          conePoints[2];

  // Member functions - private
  TruncatedCone(const TruncatedCone&);                 // Prevent copy-construction
  TruncatedCone& operator=(const TruncatedCone&);      // Prevent assignment
};


// =======================================================================================

#endif




