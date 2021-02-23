// Copyright Staniford Systems.  All Rights Reserved.  Jun 2020 -
// This class is not used directly, but instead is a superclass for anything that
// should be displayed and be stored in the quadtree


#ifndef VISUAL_OBJECT_H
#define VISUAL_OBJECT_H

#include "BoundingBox.h"
#include "TriangleBuffer.h"

// =======================================================================================
// Class variable initialization


class VisualObject
{
 public:
  
  // Instance variables - public
  BoundingBox*  box;
  float         altitude;

  // Member functions - public
  VisualObject(bool absHeights, BoundingBox* B = NULL);
  virtual ~VisualObject(void);
  void                setNoTexColor(vec3 color);
  virtual bool        getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual int         getNextIndex(bool resetToFirst);
  virtual bool        bufferGeometry(TriangleBuffer* T);
  virtual bool        matchRay(vec3& position, vec3& direction, float& lambda);
  virtual void        updateBoundingBox(void);
  virtual void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  virtual const char* objectName(void);
  virtual bool        diagnosticHTML(HttpDebug* serv);

 protected:
  
  // Instance variables - protected
  vec3          noTexColor;
  bool          useNoTexColor;
  bool          absoluteHeights;  // if true, heights are absolute.  Otherwise, they
                                  // are relative to local ground level.

private:
  
  // Member functions - private
  VisualObject(const VisualObject&);                 // Prevent copy-construction
  VisualObject& operator=(const VisualObject&);      // Prevent assignment

};

#endif




