// Copyright Staniford Systems.  All Rights Reserved.  July 2023 -

#ifndef BIO_CLASS_H
#define BIO_CLASS_H

#include "Global.h"


// =======================================================================================
/// @brief Models a Class in the biological sense as part of a Taxonomy.
///
/// Inherits from a map of Orders.

class BioClass
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  BioClass(void);
  ~BioClass(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(BioClass);
};


// =======================================================================================

#endif




