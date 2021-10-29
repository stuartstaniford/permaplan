// Copyright Staniford Systems.  All Rights Reserved.  October 2021 -

#ifndef UNIT_CUBE_FUNCTION_H
#define UNIT_CUBE_FUNCTION_H


// =======================================================================================
/// @brief This is a template for single parameter functions that map the interval 
/// [0,1] on the x-axis to the interval [0,1] on the y-axis.  
/// 
/// A variety of more specific functional forms inherit from this and are used for 
/// various empirical functional forms for tree growth etc.

class unitCubeFunction
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  unitCubeFunction(void);
  ~unitCubeFunction(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  unitCubeFunction(const unitCubeFunction&);       
  /// @brief Prevent assignment.
  unitCubeFunction& operator=(const unitCubeFunction&);      
};


// =======================================================================================

#endif




