// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -

#ifndef VISUAL_ELEMENT_H
#define VISUAL_ELEMENT_H

#include "TriangleBuffer.h"
#include "BoundingBox.h"

#ifndef VISUAL_ELEMENT_IMPLEMENTATION
extern vec3 zeroVec;
#endif

// =======================================================================================
/// @brief Superclass for renderable parts of objects.
/// 
/// This class is not used directly, but instead is a superclass for anything that might 
/// need to be assembled into TriangleBuffers for display on the GPU.  This class is 
/// typically for "parts" of things - ie they don't have their own bounding box and can't 
/// be directly stored in the Quadtree themselves, but they may be elements of larger 
/// objects which can be.  Their co-ordinates are expected to be in some frame relative 
/// to the larger object of which they are part, so they have to be supplied an offset 
/// location before operations like buffering geometry, matching rays, etc.

class VisualElement
{
 public:
  
  // Member functions - public
  VisualElement(void);
  virtual ~VisualElement(void);
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

 protected:
  
  // Instance variables - protected

private:
  
  // Member functions - private
  VisualElement(const VisualElement&);                 // Prevent copy-construction
  VisualElement& operator=(const VisualElement&);      // Prevent assignment
};

#endif




