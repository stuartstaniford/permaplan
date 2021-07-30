// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef BUILDING_H
#define BUILDING_H

#include "ObjectGroup.h"


// =======================================================================================
/// @brief Combine multiple building components into a composite entity representing the 
/// whole building.
///
/// This class exists to allow us to track the relationship of a series of gables, sheds,
/// etc that are managed as a single things (unless disaggregated to manage the
/// relationship of the individual pieces.

class Building: public ObjectGroup
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Building(VisualObject* firstComponent);
  ~Building(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  Building(const Building&);       
  /// @brief Prevent assignment.
  Building& operator=(const Building&);      

};

#endif




