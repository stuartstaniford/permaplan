// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree


#ifndef VISUAL_OBJECT_H
#define VISUAL_OBJECT_H

#include "BoundingBox.h"
#include "TriangleBuffer.h"

// =======================================================================================
// Class variable initialization

class LandSurfaceRegion;
class Quadtree;

class VisualObject
{
  friend Quadtree;
 public:
  
  // Instance variables - public
  BoundingBox*  box;

  // Member functions - public
  VisualObject(bool absHeights, BoundingBox* B = NULL);
  virtual ~VisualObject(void);
  void                setNoTexColor(vec3 color);
  virtual void        setAltitude(LandSurfaceRegion* surface);
  virtual bool        getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual int         getNextIndex(bool resetToFirst);
  virtual void        getGroundContact(float& x, float& y);
  virtual bool        bufferGeometry(TriangleBuffer* T);
  virtual bool        matchRay(vec3& position, vec3& direction, float& lambda);
  virtual void        updateBoundingBox(void);
  virtual void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  virtual const char* objectName(void);
  virtual bool        diagnosticHTML(HttpDebug* serv);
#ifdef LOG_TREE_VALIDATION
  virtual void        selfValidate(unsigned l);
#endif

 protected:
  
  // Instance variables - protected
  vec3          noTexColor;
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




