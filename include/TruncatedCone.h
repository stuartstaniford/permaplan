// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// Class for rendering a section of a cone cut off at right angles at both ends.
// Used for rendering sections of tree-trunks and the like.

#ifndef TRUNCATED_CONE_H
#define TRUNCATED_CONE_H

#include "VisualObject.h"

// =======================================================================================
// Class variable initialization

class TruncatedCone: public VisualElement
{
 public:
  
  // Instance variables - public
  vec3      location;
  vec3      axisDirection;
  float     bigRadius;
  float     smallRadius;
  unsigned  sides;
  unsigned  color;
#ifdef LOG_TREE_MATCH_RAY
  float     lastRayMatch;
  vec3      joinLine;
  vec3      originDiff;
#endif

  // Member functions - public
  TruncatedCone(vec3 root, vec3 dir, float smallR, float bigR, unsigned S);
  ~TruncatedCone(void);
  bool          updateBoundingBox(BoundingBox* box, vec3 offset);
  bool          bufferGeometry(TriangleBuffer* T, vec3 offset);
  bool          matchRay(vec3& position, vec3& direction, float& lambda, vec3 offset);
  void          triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  void          lengthen(float increment);
  void          setLength(float length);
  inline float  getLength(void) {return glm_vec3_norm(axisDirection);}
  int           printOPSF(char*& buf, unsigned bufSize);
  const char*   objectName(void);
  bool          diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  TruncatedCone(const TruncatedCone&);                 // Prevent copy-construction
  TruncatedCone& operator=(const TruncatedCone&);      // Prevent assignment
};


// =======================================================================================
// Utility function declarations

void  getCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius);


#endif




