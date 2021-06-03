// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for situations where we need to have pointers to a range of
// different things, and resolve at run time which kind they are.  This class
// provides means to do that.

#ifndef DYNAMICALLY_TYPABLE_H
#define DYNAMICALLY_TYPABLE_H

// =======================================================================================
// Class variable initialization

class DynamicallyTypable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  DynamicallyTypable(void);
  ~DynamicallyTypable(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  DynamicallyTypable(const DynamicallyTypable&);                 // Prevent copy-construction
  DynamicallyTypable& operator=(const DynamicallyTypable&);      // Prevent assignment

};

#endif




