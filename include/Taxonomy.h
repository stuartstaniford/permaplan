// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -

#ifndef TAXONOMY_H
#define TAXONOMY_H

#include "Global.h"


// =======================================================================================
/// @brief Models a particular biological classification system.  
///
/// This class should be used for common features, while subclasses should deal with
/// the details of parsing particular sources of taxonomic information. 

class Taxonomy
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Taxonomy(void);
  ~Taxonomy(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(Taxonomy);
};


// =======================================================================================

#endif




