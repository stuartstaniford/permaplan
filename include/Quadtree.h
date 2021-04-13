// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -
// This manages the quadtree used for efficient organization of visual rendering


#ifndef QUADTREE_H
#define QUADTREE_H

#include "Shader.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "DisplayList.h"
#include "LandSurfaceRegionPlanar.h"
#include <cglm/cglm.h>
#include <algorithm>
#include <list>

// =======================================================================================
// Class variable initialization


class Quadtree: public Lockable
{
  friend class LandSurface;

 public:
  
  // Instance variables - public
  
  //XXX deprecate
  unsigned      landVBOSize;        //amount of memory required for land vertices,
  unsigned      bufferOffset;       // our offset location in gpu serialized buffer
  
  unsigned      vertexTBufSize;     //amount of memory required for vertices in triangle buffer,
  unsigned      indexTBufSize;     //amount of memory required for indices in triangle buffer,
  BoundingBox   bbox;

  // Member functions - public
  Quadtree(float x, float y, unsigned width, unsigned height, float s, float t,
           float sWidth, float tHeight, unsigned& minSize, unsigned offset,
           unsigned lev, Quadtree* prt);
  ~Quadtree(void);
  void bufferGeometry(Vertex* buf);
  void bufferGeometryLeaf(Vertex* buf);
  void adjustAltitudes(LandSurfaceRegion* landsurface);
  void bufferVisualObjects(TriangleBuffer* tbuf);
  void bufferLandSurface(TriangleBuffer* tbuf);
  void storeVisualObject(VisualObject* obj);
  bool removeVisualObject(VisualObject* obj);
  void notifyObjectBoxChange(VisualObject* obj);
  void newObjectFromChild(VisualObject* obj);
  void recomputeBoundingBox(void);
  void rebuildTBufSizes(void);
  void redoLandPlanar(vec3 plane);
  void stripSurface(void);
  VisualObject* matchRay(vec3& position, vec3& direction, float& lambda);
  void saveSurfaceState(char* fileName);
  bool quadSearchHTML(HttpDebug* serv, char* path);
  bool diagnosticHTML(HttpDebug* serv, char* path);
#ifdef LOG_TREE_VALIDATION
  void selfValidate(unsigned l);
#endif


 private:
  
  // Instance variables - private
 
  //XXX deprecate
  float                     topLeftZ;
  float                     bottomRightZ;
  
  vec2                      textureBL;    // bottom left of our region in texture s-t space
  vec2                      textureTR;    // top right corner of our region in texture s-t space
  Quadtree*                 kids[4];      // our four child quadtree nodes
  Quadtree*                 parent;       // our parent
  LandSurfaceRegion*        surface;
  DisplayList               vObjects;     // objects for display that we own
  unsigned char             level;        // zero at root, increasing down tree
  bool                      isLeaf;

  // Member functions - private
  VisualObject* matchChild(vec3& position, vec3& direction, float& lambda);
  Quadtree(const Quadtree&);                 // Prevent copy-construction
  Quadtree& operator=(const Quadtree&);      // Prevent assignment
};

#endif




