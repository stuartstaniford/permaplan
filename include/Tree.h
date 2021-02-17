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
#include "TreePart.h"

#define TREE_ARRAY_SIZE   16384 // cannot exceed size of unsigned short

// =======================================================================================
// Class variable initialization

class WoodySegment;

class Tree: public VisualObject
{
  friend WoodySegment;
  
 public:
  
  // Instance variables - public
  mat4            trans;
  Species*        species;
  float           yearPlanted;
  unsigned short  treePtrArrayIndex; // used by all our subparts to find us

  // Member functions - public
  Tree(Species* S, vec3 loc, float age, float now);
  Tree(rapidjson::Value& plantObject);
  ~Tree(void);
  void        growStep(float years);
  bool        getNextUniqueVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  bool        getNextVertex(bool resetToFirst, Vertex* v, VertexDetail detail);
  int         getNextIndex(bool resetToFirst);
  bool        bufferGeometry(TriangleBuffer* T);
  void        draw(void);
  bool        matchRay(vec3& position, vec3& direction, float& lambda);
  //void        updateBoundingBox(void);
  void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  void        writeToOLDF(FILE* file, char* indent);
  const char* objectName(void);
  bool        diagnosticHTMLRow(HttpDebug* serv);
  bool        diagnosticHTML(HttpDebug* serv);

 public:
  // static functions and variables
  static void readTreesFromDesign(void);
  static void growAllTrees(float years);
  static void writeTreesToOLDF(FILE* file, char* indent);
  static bool allTreeDiagnosticHTML(HttpDebug* serv);
  static bool treePageGateway(HttpDebug* serv, char* path);

 private:
  
  // Instance variables - private
  vec3      location;
  TreePart* trunk;
  
  // static array used to allow a short index from treeParts
  static Tree** treePtrArray;
  static unsigned short treeCount;

  // Member functions - private
  Tree(const Tree&);                 // Prevent copy-construction
  Tree& operator=(const Tree&);      // Prevent assignment
};

#endif




