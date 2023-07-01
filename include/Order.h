// Copyright Staniford Systems.  All Rights Reserved.  July 2023 -

#ifndef ORDER_H
#define ORDER_H

#include "Global.h"


// =======================================================================================
/// @brief Models a biological order.
///
/// Inherits from an unordered map of names to Families.

class Order
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Order(void);
  ~Order(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(Order);
};


// =======================================================================================

#endif




