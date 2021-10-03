// Copyright Staniford Systems.  All Rights Reserved.  Dec 2020 -

#ifndef TREE_H
#define TREE_H

#include "VisualObject.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
#include "TreePart.h"
#include "Quadtree.h"

#define TREE_ARRAY_SIZE   16384 // cannot exceed size of unsigned short
#define OPACITY_ESTIMATE_FACTOR 10


// =======================================================================================
// Necessary forward declarations

class WoodySegment;
class Species;
class TaskQueue;
class Scene;

void growOneTree(void* arg);


// =======================================================================================
/// @brief Store and render trees.
///
/// This is the main class for rendering a tree as a VisualObject.  Relies heavily on
/// Species (which captures the OTDL description of each type of tree), and WoodySegment
/// which models indidual pieces of branches etc.

class Tree: public VisualObject
{
  friend WoodySegment;
  friend void growOneTree(void* arg, TaskQueue* T);

 public:
  
  // Instance variables - public
  mat4            trans;
  Species*        species;
  float           yearPlanted;
  float           ageNow;
  unsigned short  treePtrArrayIndex; // used by all our subparts to find us
  short           taskId;             // which thread has us
  float           trunkRadiusObserved;
  float           yearTrunkMeasured;
  const char*     commonName;
  const char*     taxonomyLink;

  // Member functions - public
  Tree(Species* S, vec3 loc, float age, float now);
  Tree(rapidjson::Value& plantObject);
  ~Tree(void);
  void        growStep(float years);
  void        updateBoundingBox(void);
  float       getHeight(void);
  float       getRadius(void);
  bool        bufferGeometryOfObject(TriangleBuffer* T);
  void        getGroundContact(float& x, float& y);
  bool        matchRayToObject(vec3& position, vec3& direction, float& lambda);
  //void        updateBoundingBox(void);
  float       estimateOpacity(vec3 direction);
  void        triangleBufferSizes(unsigned& vCount, unsigned& iCount);
  void        writeToOLDF(FILE* file, char* indent);
  const char* objectName(void);
  virtual DynamicType getDynamicType(void) {return TypeTree;}
  bool        diagnosticHTMLSummary(HttpDebug* serv);
  bool        diagnosticHTMLRow(HttpDebug* serv);
  bool        diagnosticHTML(HttpDebug* serv);
#ifdef LOG_TREE_VALIDATION
  void        selfValidate(unsigned l);
#endif

 public:
  // static functions and variables
  static void readTreesFromDesign(Quadtree* qtree);
#ifdef MULTI_THREADED_SIMULATION
  static void simulationThreadBase(int s);
  static void analyzeTreeGraph(float years, Scene& scene);
#else
  static void growAllTrees(float years);
#endif
  static void writeTreesToOLDF(FILE* file, char* indent);
  static bool allTreeDiagnosticHTML(HttpDebug* serv);
  static bool treePageGateway(HttpDebug* serv, char* path);

 private:
  
  // Instance variables - private
  vec3      location;
  TreePart* trunk;
  float     yearsToSim;
  
  // static array used to allow a short index from treeParts
  static Tree** treePtrArray;
  static unsigned short treeCount;

  // Member functions - private
  float estimateOpacityAxially(int axis);
  Tree(const Tree&);                 // Prevent copy-construction
  Tree& operator=(const Tree&);      // Prevent assignment
};

#endif




