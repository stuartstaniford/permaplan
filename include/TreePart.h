// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class is an abstract interface for all parts of trees.

#ifndef TREE_PART_H
#define TREE_PART_H

#include "HttpDebug.h"

// =======================================================================================
// Class variable initialization


class TreePart
{
 public:
  
  // Member functions - public
  TreePart(void);
  virtual ~TreePart(void);
  virtual const char* objectName(void);
  virtual bool        diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  TreePart(const TreePart&);                 // Prevent copy-construction
  TreePart& operator=(const TreePart&);      // Prevent assignment
};

#endif




