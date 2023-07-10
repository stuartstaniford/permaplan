// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -

#ifndef FAMILY_H
#define FAMILY_H

#include "Global.h"
#include <string>
#include <unordered_map>


// =======================================================================================
// Forward declarations

class Genus;
class HttpServThread;


// =======================================================================================
/// @brief Holder for a taxonomic family (containing genera, which contain species).
///
/// Inherits from an unorderded map of Genera.

class Family: public std::unordered_map<std::string, Genus*>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Family(char* genusName);
  ~Family(void);
  bool add(char* species, char* genus);
  bool generaHTMLTable(HttpServThread* serv);

private:
  
  // Instance variables - private
  char* name;

  // Member functions - private
  PreventAssignAndCopyConstructor(Family);
};


// =======================================================================================

#endif




