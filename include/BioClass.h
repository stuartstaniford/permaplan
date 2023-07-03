// Copyright Staniford Systems.  All Rights Reserved.  July 2023 -

#ifndef BIO_CLASS_H
#define BIO_CLASS_H

#include "Global.h"
#include <string>
#include <unordered_map>


// =======================================================================================
// Forward declarations

class Order;
class HttpServThread;


// =======================================================================================
/// @brief Models a Class in the biological sense as part of a Taxonomy.
///
/// Inherits from a map of Orders.

class BioClass: public std::unordered_map<std::string, Order*>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  BioClass(void);
  ~BioClass(void);
  bool add(char* species, char* genus, char* family, char* order);
  bool orderHTMLTable(HttpServThread* serv);

private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(BioClass);
};


// =======================================================================================

#endif




