// Copyright Staniford Systems.  All Rights Reserved.  July 2020 -
// Class for storing and rendering a section of the landsurface mesh for some
// particular region in the quadtree (generally a leaf).  This class defines
// the interface that any implementation providing this service needs to conform to.

#ifndef LAND_SURFACE_REGION_H
#define LAND_SURFACE_REGION_H

#include <vector>
#include "VisualObject.h"

// =======================================================================================
// Class variable initialization


class LandSurfaceRegion: public VisualObject
{
 public:
  
  // Instance variables - public
  vec2 xyPos;
  vec2 extent;
  vec2 stPos;
  vec2 stExtent;

  // Member functions - public
  LandSurfaceRegion(float x, float y, float width, float height, float s, float t,
                    float sWidth, float tHeight);
  virtual ~LandSurfaceRegion(void);
  virtual bool        getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  virtual int         getNextIndex(bool resetToFirst);
  virtual bool        bufferGeometry(TriangleBuffer* T);
  virtual bool        matchRayToObject(vec3& position, vec3& direction, float& lambda);
  virtual float       getAltitude(float x, float y);
  virtual void        updateBoundingBox(void);
  virtual void        triangleBufferSize(unsigned& vCount, unsigned& iCount);
  virtual void        fit(std::vector<float*>& locations);
  virtual const char* objectName(void);
  virtual bool        diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  LandSurfaceRegion(const LandSurfaceRegion&);                 // Prevent copy-construction
  LandSurfaceRegion& operator=(const LandSurfaceRegion&);      // Prevent assignment
};

#endif




