// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -

#ifndef EMPTY_CLASS_H
#define EMPTY_CLASS_H


// =======================================================================================
/// @brief Quick description of the class
///
/// More details of the class go here.

class EmptyClass
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  EmptyClass(void);
  ~EmptyClass(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  EmptyClass(const EmptyClass&);       
  /// @brief Prevent assignment.
  EmptyClass& operator=(const EmptyClass&);      

};

#endif




