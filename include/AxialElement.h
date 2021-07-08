// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -

#ifndef AXIAL_ELEMENT_H
#define AXIAL_ELEMENT_H

#include "VisualElement.h"


// =======================================================================================
/// @brief Specialization of VisualElement specifically for any element that can be 
/// modeled as sweeping a vector path around an axis. 
///  
/// Things like cylinder, cone, etc can inherit from this and don't require very much
/// code on top of this.  It can only model things that have full 360 degree rotational
/// symmetry around an axis.


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

  // Member functions - protected
 protected:
  AxialElement(vec3 root, vec3 dir, float R, unsigned sides);

  // Member functions - public  
 public:
  ~AxialElement(void);
  
  // The VisualElement API
  virtual bool        bufferGeometryOfElement(TriangleBuffer* T, vec3 offset);
  virtual bool        matchRayToElement(vec3& position, vec3& direction, float& lambda, vec3 offset);
  virtual bool        updateBoundingBox(BoundingBox* box, vec3 offset);
  virtual void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  virtual const char* elementName(void);
  virtual bool        getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual int         getNextIndex(bool resetToFirst);
  virtual bool        diagnosticHTML(HttpDebug* serv);
  virtual bool        diagnosticHTMLSummary(HttpDebug* serv);


  // Additional functions we define
  void                lengthen(float increment);
  void                setLength(float length);
  virtual int         printOPSF(char*& buf, unsigned bufSize);

  ///@brief Interface to return the objectIndex if we have one
  ///@returns 0, subclasses with a valid objIndex should override
  inline virtual unsigned getObjectIndex(void) {return 0u;} 

  ///@brief Provide the length of the axis
  ///@returns The length as a float value.
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




