// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -

#ifndef TAXONOMY_H
#define TAXONOMY_H

#include "Global.h"
#include <string>
#include <map>
#include <unordered_map>

#define MAX_SPECIES_PATH     256  // the following considers genus/species, then we allow
                                   // a fudge factor for variety names.
                                   //https://en.wikipedia.org/wiki/List_of_long_species_names

// =======================================================================================
// Forward declarations

class Species;
class Genus;
class Family;
class BioClass;


// =======================================================================================
/// @brief Container class for a collection of Species indexed by a string name.

class SpeciesList: public std::unordered_map<std::string, Species*>
{
  
};


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
  bool add(char* species, char* genus, char* family, char* order, char* bioClass);
  
private:
  
  // Instance variables - private
  static std::unordered_map<std::string, unsigned> genusList;
  static std::unordered_map<std::string, SpeciesList*> genusSpeciesList;
  static std::unordered_map<std::string, BioClass> bioClasses;
  
  // Member functions - private
  PreventAssignAndCopyConstructor(Taxonomy);
};


// =======================================================================================

#endif




