// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef REGION_LIST_H
#define REGION_LIST_H

#include "DynamicallyTypable.h"
#include <unordered_map>
#include <string>


// =======================================================================================
/// @brief This class is for managing lists of species and lists of lists of species 
/// for use in selecting species based on region/type.
///
/// Inherits from an unordered_map from string names to DynamicallyTypable*, which will
/// point to either other RegionLists, or SpeciesPath entries which store attributes
/// of species that need to be offered in this particular tree of RegionLists.

class RegionList: public std::unordered_map<std::string, DynamicallyTypable*>, 
                  public DynamicallyTypable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  RegionList(const char* fileName);
  ~RegionList(void);
  inline DynamicType getDynamicType(void) {return TypeRegionList;}

  // Static public functions
  static void loadRoot(void);
  static RegionList& getRoot(void);
  
private:
  
  // Instance variables - private
  static RegionList* root;
  
  // Member functions - private
  RegionList(const RegionList&);                 // Prevent copy-construction
  RegionList& operator=(const RegionList&);      // Prevent assignment
};


// =======================================================================================

#endif




