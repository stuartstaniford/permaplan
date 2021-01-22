// Copyright Staniford Systems.  All Rights Reserved.  Jan 2021 -
// This class manages a segment of a trunk/branch/stem of a woody plant or tree.  It
// models the heartwood/sapwood/bark, etc.

#ifndef WOODY_SEGMENT_H
#define WOODY_SEGMENT_H

#include "TreePart.h"

// =======================================================================================
// Class variable initialization


class WoodySegment: public TreePart
{
 public:
  
  // Member functions - public
  WoodySegment(void);
  ~WoodySegment(void);
  const char* objectName(void);
  bool        diagnosticHTML(HttpDebug* serv);

 private:
  
  // Instance variables - private
  
  // Member functions - private
  WoodySegment(const WoodySegment&);                 // Prevent copy-construction
  WoodySegment& operator=(const WoodySegment&);      // Prevent assignment
};

#endif




