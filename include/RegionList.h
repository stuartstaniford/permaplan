// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for managing lists of species and lists of lists of species for use
// in selecting based on region/type

#ifndef REGION_LIST_H
#define REGION_LIST_H

#include "DynamicallyTypable.h"
#include <unordered_map>
#include <string>


// =======================================================================================
// Class variable initialization

class RegionList: public std::unordered_map<std::string, RegionList*>, 
                  public DynamicallyTypable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  RegionList(const char* fileName);
  ~RegionList(void);
  static void loadRoot(void);
  static RegionList& getRoot(void);
  inline DynamicType getDynamicType(void) {return TypeRegionList;}

private:
  
  // Instance variables - private
  static RegionList* root;
  
  // Member functions - private
  RegionList(const RegionList&);                 // Prevent copy-construction
  RegionList& operator=(const RegionList&);      // Prevent assignment

};

#endif




