// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -

#ifndef WOODY_SEGMENT_H
#define WOODY_SEGMENT_H

#include "TreePart.h"
#include <vector>

#define WOOD_SEG_SIDES 10   //XX initial hack that needs to be made more LOD


// =======================================================================================
// Forward declarations

class Tree;
class AxialElement;


// =======================================================================================
/// @brief Model a segment of branch/trunk in a plant.
/// 
/// This class manages a segment of a trunk/branch/stem of a woody plant or tree.  It
/// models the heartwood/sapwood/bark, etc.

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
  bool        matchRay(vec3& position, vec3& direction, vec3 offset);
  const char* objectName(void);
  bool        diagnosticHTML(HttpDebug* serv);
  void        selfValidate(unsigned l);

 private:
  float                   heartRadius;    // expressed in mm
  float                   sapThickness;   // mm
  float                   barkThickness;  // mm.  Phloem currently part of bark
  unsigned                barkColor;
  unsigned short          level;
  AxialElement*            cylinder;
  std::vector<TreePart*>  kids;
  
  // Instance variables - private
  
  // Member functions - private
  WoodySegment(const WoodySegment&);                 // Prevent copy-construction
  WoodySegment& operator=(const WoodySegment&);      // Prevent assignment
};


// =======================================================================================

#endif




