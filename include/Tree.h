// Copyright Staniford Systems.  All Rights Reserved.  Dec 2020 -
// Class for storing and rendering trees.

#ifndef TREE_H
#define TREE_H

#include "VisualObject.h"
#include "Species.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

#define TREE_ARRAY_SIZE   16384 // cannot exceed size of unsigned short
#define MAX_SPECIES_PATH  96   //https://en.wikipedia.org/wiki/List_of_long_species_names

// =======================================================================================
// Class variable initialization

class WoodySegment;

class Tree: public VisualObject
{
  friend WoodySegment;
  
 public:
  
  // Instance variables - public
  mat4            trans;
  char            speciesPath[MAX_SPECIES_PATH];
  unsigned short  treePtrArrayIndex; // used by all our subparts to find us
  Species*        species;

  // Member functions - public
  Tree(rapidjson::Value& plantObject);
  // We take the unit box on [0,0,0] to [1,1,1] and apply transform to it.
  // Tree(mat4 transform);
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

 public:
  // static functions and variables
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




