// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -

#ifndef FAMILY_H
#define FAMILY_H

#include "Global.h"


// =======================================================================================
/// @brief Holder for a taxonomic family (containing genera, which contain species).
///

class Family
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Family(void);
  ~Family(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(Family);
};


// =======================================================================================

#endif




