// Copyright Staniford Systems.  All Rights Reserved.  May 2020 -

#ifndef QUADTREE_H
#define QUADTREE_H

#include "BoundingBox.h"
#include "DisplayList.h"
#include "Lockable.h"
#include <cglm/cglm.h>
#include <algorithm>
#include <list>


// =======================================================================================
// Forward declarations

// =======================================================================================
/// @brief This manages the quadtree used for efficient organization of visual objects 
/// for rendering. 
/// 
/// This class is one of the most important in the codebase and is the central 
/// geographical data structure used for organizing all the renderable VisualObjects so
/// that we can effiently decide which ones might be in camera-view, might intersect a
/// ray, etc.  See [the Wikipedia entry](https://en.wikipedia.org/wiki/Quadtree) for 
/// general background on quadtrees.

class Quadtree: public Lockable
{
  friend class LandSurface;

 public:
  
  // Instance variables - public
  
  //XXX deprecate
  unsigned      landVBOSize;        //amount of memory required for land vertices,
  unsigned      bufferOffset;       // our offset location in gpu serialized buffer
  
  LandSurfaceRegion*  surface;
  unsigned            vertexTBufSize; //amount of memory required for vertices in triangle buffer,
  unsigned            indexTBufSize; //amount of memory required for indices in triangle buffer,
  BoundingBox         bbox;

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
  bool quadSearchHTML(HttpDebug* serv, char* searchTerm);
  bool quadSearchRecursive(HttpDebug* serv, int& nextRow, char* searchTerm, char* quadPath);
  bool diagnosticHTML(HttpDebug* serv, char* path);
  void selfValidate(unsigned l);


 private:
  
  // Instance variables - private
 
  //XXX deprecate
  float                     topLeftZ;
  float                     bottomRightZ;
  
  vec2                      textureBL;    // bottom left of our region in texture s-t space
  vec2                      textureTR;    // top right corner of our region in texture s-t space
  Quadtree*                 kids[4];      // our four child quadtree nodes
  Quadtree*                 parent;       // our parent
  DisplayList               vObjects;     // objects for display that we own
  unsigned char             level;        // zero at root, increasing down tree
  bool                      isLeaf;

  // Member functions - private
  VisualObject* matchChild(vec3& position, vec3& direction, float& lambda);
  Quadtree(const Quadtree&);                 // Prevent copy-construction
  Quadtree& operator=(const Quadtree&);      // Prevent assignment
};

#endif




