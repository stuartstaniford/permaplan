// Copyright Staniford Systems.  All Rights Reserved.  July 2023 -

#ifndef ORDER_H
#define ORDER_H

#include "Global.h"
#include <string>
#include <unordered_map>


// =======================================================================================
// Forward declarations

class HttpServThread;
class Family;


// =======================================================================================
/// @brief Models a biological order.
///
/// Inherits from an unordered map of names to Families.

class Order: public std::unordered_map<std::string, Family*>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Order(char* orderName);
  ~Order(void);
  bool add(char* species, char* genus, char* family);
  bool provideOrderPage(HttpServThread* serv);

private:
  
  // Instance variables - private
  char* name;
  
  // Member functions - private
  PreventAssignAndCopyConstructor(Order);
};


// =======================================================================================

#endif




