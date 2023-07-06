// Copyright Staniford Systems.  All Rights Reserved.  July 2023 -

#ifndef ORDER_H
#define ORDER_H

#include "Global.h"


// =======================================================================================
// Forward declarations

class HttpServThread;


// =======================================================================================
/// @brief Models a biological order.
///
/// Inherits from an unordered map of names to Families.

class Order
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Order(char* orderName);
  ~Order(void);
  bool provideOrderPage(HttpServThread* serv);

private:
  
  // Instance variables - private
  char* name;
  
  // Member functions - private
  PreventAssignAndCopyConstructor(Order);
};


// =======================================================================================

#endif




