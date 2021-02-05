// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class manages a segment of a trunk/branch/stem of a woody plant or tree.  It
// models the heartwood/sapwood/bark, etc.

#ifndef WOODY_SEGMENT_H
#define WOODY_SEGMENT_H

#include "TreePart.h"

#define WOOD_SEG_SIDES 10   //XX initial hack that needs to be made more LOD

// =======================================================================================
// Class variable initialization

class Tree;

class WoodySegment: public TreePart
{
  friend Tree;
  
 public:
  
  // Member functions - public
  WoodySegment(Species& species, unsigned short treeIndex, float years);
  ~WoodySegment(void);
  bool bufferGeometry(TriangleBuffer* T);
 void triangleBufferSizesRecurse(unsigned& vCount, unsigned& iCount);
  const char* objectName(void);
  bool        diagnosticHTML(HttpDebug* serv);

 private:
  float           length;         // mm
  float           heartRadius;    // expressed in mm
  float           sapThickness;   // mm
  float           barkThickness;  // mm.  Phloem currently part of bark

  // Instance variables - private
  
  // Member functions - private
  WoodySegment(const WoodySegment&);                 // Prevent copy-construction
  WoodySegment& operator=(const WoodySegment&);      // Prevent assignment
};

#endif




