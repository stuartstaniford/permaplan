// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is an abstract interface for all parts of trees.

#ifndef TREE_PART_H
#define TREE_PART_H

#include "HttpDebug.h"
#include "Species.h"
#include "TriangleBuffer.h"

// =======================================================================================
// Class variable initialization


class TreePart
{
 public:
  
  // Member functions - public
  TreePart(unsigned short treeIndex);
  virtual ~TreePart(void);
  virtual bool bufferGeometry(TriangleBuffer* T, float altitude);
  virtual void triangleBufferSizesRecurse(unsigned& vCount, unsigned& iCount);
  virtual void growStep(float years);

  virtual const char* objectName(void);
  virtual bool diagnosticHTML(HttpDebug* serv);

 protected:
  
  // Instance variables - protected
  unsigned short ourTreeIndex;

private:

  // Member functions - private
  TreePart(const TreePart&);                 // Prevent copy-construction
  TreePart& operator=(const TreePart&);      // Prevent assignment
};

#endif




