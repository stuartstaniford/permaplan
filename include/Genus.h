// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -

#ifndef GENUS_H
#define GENUS_H

#include "Global.h"
#include "Taxonomy.h"


// =======================================================================================
/// @brief Models a biological genus as part of a Taxonomy.
///
///

class Genus: public SpeciesList
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Genus(void);
  ~Genus(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(Genus);
};


// =======================================================================================

#endif




