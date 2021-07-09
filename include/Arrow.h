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
/// Note that this is an instance of a common pattern in the code where an object type
/// inherits significant functionality from a subtype of VisualElement (in this case
/// AxialElement) which does a lot of the hard work of the geometry, but also inherits
/// from VisualObject, because an Arrow is an entire object.  Since VisualObject is also
/// a child of VisualElement, we inherit from that via two paths.  In some cases, this
/// requires some explicit hard-wiring of exactly which way the inheritance goes.

class Arrow: public AxialElement, public VisualObject
{
 public:
  
  // Instance variables - public
  
  // Member functions - public
  Arrow(vec3 root, vec3 dir, float radius, int sides);
  ~Arrow(void);
  
  static vec2 arrowPoints[3];
  
  void selfValidate(unsigned l);
  const char* objectName(void);
  bool        diagnosticHTMLSummary(HttpDebug* serv);
  bool        diagnosticHTML(HttpDebug* serv);

  /// @brief Disgorge our geometry to a TriangleBuffer.
  /// 
  /// We just invoke AxialElement::bufferGeometryOfObject.
  /// @param T A pointer to a TriangleBuffer into which the arrow should insert its
  /// vertices and indices.
  inline bool bufferGeometryOfObject(TriangleBuffer* T)
    {return AxialElement::bufferGeometryOfElement(T, zeroVec);}
  
  /// @brief The amount of vertex and index space we would need in a TriangleBuffer.
  /// 
  /// We just invoke AxialElement::triangleBufferSizes.
  /// @param vCount A reference to a count which will hold the number of Vertex objects 
  /// that will be generated.
  /// @param iCount A reference to a count which will hold the number of unsigned indices 
  /// that will be generated.  
  inline void triangleBufferSizes(unsigned& vCount, unsigned& iCount)
    {AxialElement::triangleBufferSizes(vCount, iCount);}
  
  /// @brief Decide if a ray touches us.  
  /// 
  /// Function to decide whether a given line touches the object or not. We just invoke 
  /// AxialElement::matchRayToElement
  /// @param position The vec3 for a point on the ray to be matched.
  /// @param direction The vec3 for the direction of the ray.
  /// @param lambda A reference to a float which will be used to store the multiple of 
  /// the direction vector from the position to the match point on the object.
  inline bool matchRayToObject(vec3& position, vec3& direction, float& lambda)
    {return AxialElement::matchRayToElement(position, direction, lambda, zeroVec);}

  /// @brief Return our objIndex
  inline virtual unsigned getObjectIndex(void) {return objIndex;};

 private:
  
  // Instance variables - private

  // Member functions - private
  Arrow(const Arrow&);                 // Prevent copy-construction
  Arrow& operator=(const Arrow&);      // Prevent assignment
};

#endif

