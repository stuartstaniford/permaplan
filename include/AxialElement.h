// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class is a specialization of VisualElement specifically for any element
// that can be modeled as sweeping a vector path around an axis.  Things like cylinder,
// cone, etc can inherit from this.


#ifndef AXIAL_ELEMENT_H
#define AXIAL_ELEMENT_H

#include "VisualElement.h"

// =======================================================================================
// Class variable initialization

class AxialElement: public VisualElement
{
 public:
  vec3            location;
  vec3            axisDirection;
  float           radius;
  unsigned        color;
  unsigned short  sides;
  bool            closedTop;
  bool            closedBase;

  // Member functions - public
  AxialElement(vec3 root, vec3 dir, float R, unsigned sides);
  virtual ~AxialElement(void);
  
  // The VisualElement API
  virtual bool        getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual int         getNextIndex(bool resetToFirst);
  virtual bool        bufferGeometry(TriangleBuffer* T, vec3 offset);
  virtual bool        matchRay(vec3& position, vec3& direction, float& lambda, vec3 offset);
  virtual bool        updateBoundingBox(BoundingBox* box, vec3 offset);
  virtual void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  virtual const char* elementName(void);
  virtual bool        diagnosticHTML(HttpDebug* serv);
  virtual bool        diagnosticHTMLSummary(HttpDebug* serv);

  // Additional functions we define
  void                lengthen(float increment);
  void                setLength(float length);
  inline float        getLength(void) {return glm_vec3_norm(axisDirection);}

 protected:

  // Protected functions only for use by us or subclasses
  bool          matchRayBruteForce(vec3& position, vec3& direction,
                                                            float& lambda, vec3 offset);

  // Instance variables - protected
  unsigned short  NVecs;
  vec2*           vectorPath;
  vec3            f1;    // f1 and f2 are cross-vectors to axisDirection
  vec3            f2;
#ifdef LOG_TREE_MATCH_RAY
  float           lastRayMatch;
  vec3            joinLine;
  vec3            originDiff;
#endif

private:
  
  // Member functions - private
  AxialElement(const AxialElement&);                 // Prevent copy-construction
  AxialElement& operator=(const AxialElement&);      // Prevent assignment
};

#endif

// =======================================================================================
// Utility function declarations

void  getCrossVectors(vec3 dir, vec3 f1, vec3 f2, float radius);



