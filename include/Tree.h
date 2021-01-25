// Copyright Staniford Systems.  All Rights Reserved.  Dec 2020 -
// Class for storing and rendering trees.

#ifndef TREE_H
#define TREE_H

#include "VisualObject.h"

#define TREE_ARRAY_SIZE 16384 // cannot exceed size of unsigned short

// =======================================================================================
// Class variable initialization

class WoodySegment;

class Tree: public VisualObject
{
  friend WoodySegment;
  
 public:
  
  // Instance variables - public
  mat4 trans;

  // Member functions - public
  Tree(mat4 transform);   // We take the unit box on [0,0,0] to [1,1,1] and apply transform to it.
  ~Tree(void);
  bool        getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  int         getNextIndex(bool resetToFirst);
  bool        bufferGeometry(TriangleBuffer* T);
  void        draw(void);
  bool        matchRay(vec3& position, vec3& direction, float& lambda);
  void        updateBoundingBox(void);
  void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  const char* objectName(void);
  bool        diagnosticHTML(HttpDebug* serv);

  // static functions
  static void readTreesFromDesign(void);

 private:
  
  // Instance variables - private
  char iterI, iterJ, iterK;
  int  index;
  
  // static array used to allow a short index from treeParts
  static Tree** treePtrArray;
  static unsigned short treeCount;

  // Member functions - private
  Tree(const Tree&);                 // Prevent copy-construction
  Tree& operator=(const Tree&);      // Prevent assignment
};

#endif




