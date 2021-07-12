// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef MENU_INSERT_H
#define MENU_INSERT_H


// =======================================================================================
/// @brief Code for the insertion menu that comes up on double-clicking.
///
/// The job of this class is to provide a menu capable of routing us to task-specific
/// panels for all the different kinds of things that can be insderted (height 
/// measurements, building components, trees/plants, etc).

class MenuInsert
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  MenuInsert(void);
  ~MenuInsert(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  MenuInsert(const MenuInsert&);                 // Prevent copy-construction
  MenuInsert& operator=(const MenuInsert&);      // Prevent assignment

};

#endif




