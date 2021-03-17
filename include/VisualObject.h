// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree


#ifndef VISUAL_OBJECT_H
#define VISUAL_OBJECT_H

#include "VisualElement.h"

// =======================================================================================
// Class variable initialization

class LandSurfaceRegion;
class Quadtree;

class VisualObject: public VisualElement
{
  friend Quadtree;
 public:
  
  // Instance variables - public
  BoundingBox*  box;

  // Member functions - public
  VisualObject(bool absHeights, BoundingBox* B = NULL);
  virtual ~VisualObject(void);

  // Public member functions coming from VisualElement
  inline bool updateBoundingBox(BoundingBox* B)
                    {return VisualElement::updateBoundingBox(B);}
  virtual bool        getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual int         getNextIndex(bool resetToFirst);
  virtual bool        bufferGeometry(TriangleBuffer* T);
  virtual bool        matchRay(vec3& position, vec3& direction, float& lambda);
  virtual void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  virtual const char* objectName(void);
  virtual bool        diagnosticHTML(HttpDebug* serv);
  virtual bool        diagnosticHTMLSummary(HttpDebug* serv);

  // Public member functions arising here
  void                setNoTexColor(unsigned color);
  virtual void        setAltitude(LandSurfaceRegion* surface);
  virtual void        getGroundContact(float& x, float& y);
  virtual void        updateBoundingBox(void);
#ifdef LOG_TREE_VALIDATION
  virtual void        selfValidate(unsigned l);
#endif

 protected:
  
  // Instance variables - protected
  unsigned      noTexColor;
  float         altitude;
  Quadtree*     qTreeNode;
  bool          useNoTexColor;
  bool          absoluteHeights;  // if true, heights are absolute.  Otherwise, they
                                  // are relative to local ground level.

private:
  
  // Member functions - private
  VisualObject(const VisualObject&);                 // Prevent copy-construction
  VisualObject& operator=(const VisualObject&);      // Prevent assignment

};

#endif




