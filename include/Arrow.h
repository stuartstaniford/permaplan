// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -

#ifndef ARROW_H
#define ARROW_H

#include "AxialElement.h"
#include "VisualObject.h"

// =======================================================================================
// Class variable initialization

//XX probably these shouldn't be hard coded
#define ARROW_HEAD 0.25          // fraction of the arrow in the head.
#define ARROW_HEAD_WIDTH 1.0f    // multiple of the radius for the overhang over the shaft.

///@brief Class for rendering a simple straight arrow.
///

class Arrow: public AxialElement, public VisualObject
{
 public:
  
  // Instance variables - public
  
  // Member functions - public
  Arrow(vec3 root, vec3 dir, float radius, int sides);
  ~Arrow(void);
  inline bool bufferGeometryOfObject(TriangleBuffer* T)
    {return AxialElement::bufferGeometryOfElement(T, zeroVec);}
  inline void triangleBufferSizes(unsigned& vCount, unsigned& iCount)
    {AxialElement::triangleBufferSizes(vCount, iCount);}
  inline bool matchRayToObject(vec3& position, vec3& direction, float& lambda)
    {return AxialElement::matchRayToElement(position, direction, lambda, zeroVec);}
  inline bool matchRayToElement(vec3& position, vec3& direction, float& lambda, vec3 offset)
    {return AxialElement::matchRayToElement(position, direction, lambda, offset);}
  inline virtual unsigned getObjectIndex(void) {return objIndex;};

  static vec2 arrowPoints[3];
  
  void selfValidate(unsigned l);
  
  const char* objectName(void);
  bool        diagnosticHTMLSummary(HttpDebug* serv);
  bool        diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private

  // Member functions - private
  Arrow(const Arrow&);                 // Prevent copy-construction
  Arrow& operator=(const Arrow&);      // Prevent assignment
};

#endif

