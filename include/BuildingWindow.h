// Copyright Staniford Systems.  All Rights Reserved.  August 2021 -

#ifndef BUILDING_WINDOW_H
#define BUILDING_WINDOW_H

#include "Window3D.h"


// =======================================================================================
// Useful constants etc

#define BUILDING_WINDOW_INIT_WIDTH  600
#define BUILDING_WINDOW_INIT_HEIGHT 500


// =======================================================================================
/// @brief Inspector window to allow manipulation of an individual building
///
/// This subclass of Window3D is specialized to allow interaction with a single object
/// of type Building for the purpose of messing with the building, adding gables, sheds,
/// etc.

class BuildingWindow: public Window3D
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  BuildingWindow(void);
  ~BuildingWindow(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  BuildingWindow(const BuildingWindow&);       
  /// @brief Prevent assignment.
  BuildingWindow& operator=(const BuildingWindow&);      

};

#endif



