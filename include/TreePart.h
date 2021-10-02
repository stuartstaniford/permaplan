// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -

#ifndef TREE_PART_H
#define TREE_PART_H

#include <cglm/cglm.h>
#include "Logging.h"


// =======================================================================================
// Forward declarations

class Tree;
class HttpDebug;
class Species;
class TriangleBuffer;
class BoundingBox;


// =======================================================================================
/// @brief This class is an abstract interface for all parts of trees.

class TreePart
{
  friend Tree;
  
 public:
  
  // Member functions - public
  TreePart(unsigned short treeIndex);
  virtual ~TreePart(void);
  virtual bool        bufferGeometry(TriangleBuffer* T, vec3 offset);
  virtual void        triangleBufferSizesRecurse(unsigned& vCount, unsigned& iCount);
  virtual void        growStep(float years);
  virtual bool        updateBoundingBox(BoundingBox* box, float altitude);
  virtual const char* objectName(void);
  virtual bool        diagnosticHTML(HttpDebug* serv);
  virtual bool        matchRay(vec3& position, vec3& direction, vec3 offset);
  inline  unsigned short getTreeIndex(void){return ourTreeIndex;}
#ifdef LOG_TREE_VALIDATION
  virtual void        selfValidate(unsigned l);
#endif

 protected:
  
  // Instance variables - protected
  unsigned short ourTreeIndex;

private:

  // Member functions - private
  TreePart(const TreePart&);                 // Prevent copy-construction
  TreePart& operator=(const TreePart&);      // Prevent assignment
};


// =======================================================================================

#endif




