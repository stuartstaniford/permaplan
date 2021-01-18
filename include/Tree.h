// Copyright Staniford Systems.  All Rights Reserved.  Dec 2020 -
// Class for storing and rendering trees.

#ifndef TREE_H
#define TREE_H

#include "VisualObject.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


// =======================================================================================
// Class variable initialization


class Tree: public VisualObject
{
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
  void        readOTDLFromFile(char* fileName);
  bool        validateOTDL(rapidjson::Document& doc);
  const char* objectName(void);
  bool        diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  char iterI, iterJ, iterK;
  int  index;
  
  // Member functions - private
  Tree(const Tree&);                 // Prevent copy-construction
  Tree& operator=(const Tree&);      // Prevent assignment
};

#endif




