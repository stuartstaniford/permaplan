// Copyright Staniford Systems.  All Rights Reserved.  October 2021 -

#ifndef TREE_EQUATION_H
#define TREE_EQUATION_H


// =======================================================================================
/// @brief This is a template for single parameter functions that map the interval 
/// [0,1] on the x-axis to the interval [0,1] on the y-axis.  
/// 
/// A variety of more specific functional forms inherit from this and are used for 
/// various empirical functional forms for tree growth etc.

/// See https://www.fs.usda.gov/treesearch/pubs/52933
/// and https://www.fs.usda.gov/rds/archive/Catalog/RDS-2016-0005
/// for more background information.

class TreeEquation
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  TreeEquation(void);
  ~TreeEquation(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  TreeEquation(const TreeEquation&);       
  /// @brief Prevent assignment.
  TreeEquation& operator=(const TreeEquation&);      
};


// =======================================================================================

#endif




