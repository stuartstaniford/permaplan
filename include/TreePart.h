// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is an abstract interface for all parts of trees.

#ifndef TREE_PART_H
#define TREE_PART_H

#include "HttpDebug.h"
#include "Species.h"

// =======================================================================================
// Class variable initialization


class TreePart
{
 public:
  
  // Member functions - public
  TreePart(unsigned short treeIndex);
  virtual ~TreePart(void);
  virtual void triangleBufferSizesRecurse(unsigned& vCount, unsigned& iCount);
  virtual const char* objectName(void);
  virtual bool        diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  unsigned short ourTreeIndex;

  // Member functions - private
  TreePart(const TreePart&);                 // Prevent copy-construction
  TreePart& operator=(const TreePart&);      // Prevent assignment
};

#endif




