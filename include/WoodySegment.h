// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class manages a segment of a trunk/branch/stem of a woody plant or tree.  It
// models the heartwood/sapwood/bark, etc.

#ifndef WOODY_SEGMENT_H
#define WOODY_SEGMENT_H

#include <vector>
#include "TreePart.h"
#include "Cylinder.h"

#define WOOD_SEG_SIDES 10   //XX initial hack that needs to be made more LOD

// =======================================================================================
// Class variable initialization

class Tree;

class WoodySegment: public TreePart
{
  friend Tree;
  
 public:
  
  // Member functions - public
  WoodySegment(Species& species, unsigned short treeIndex,
                                unsigned short lev, vec3 loc, vec3 dir);
  ~WoodySegment(void);
  bool        updateBoundingBox(BoundingBox* box, float altitude);
  bool        bufferGeometry(TriangleBuffer* T, vec3 offset);
  void        triangleBufferSizesRecurse(unsigned& vCount, unsigned& iCount);
  unsigned    expectedKids(float len);
  void        growStep(float years);
  int         printOPSF(char*& buf, unsigned bufSize);
  bool        matchRay(vec3& position, vec3& direction);
  const char* objectName(void);
  bool        diagnosticHTML(HttpDebug* serv);
#ifdef LOG_TREE_VALIDATION
  void        selfValidate(unsigned l);
#endif

 private:
  float                   heartRadius;    // expressed in mm
  float                   sapThickness;   // mm
  float                   barkThickness;  // mm.  Phloem currently part of bark
  unsigned                barkColor;
  unsigned short          level;
  Cylinder*               cylinder;
  std::vector<TreePart*>  kids;
  
  // Instance variables - private
  
  // Member functions - private
  WoodySegment(const WoodySegment&);                 // Prevent copy-construction
  WoodySegment& operator=(const WoodySegment&);      // Prevent assignment
};

#endif




