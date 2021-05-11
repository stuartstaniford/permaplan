// Copyright Staniford Systems.  All Rights Reserved.  May 2021 -
// This class ..... is a template for real classes

#ifndef EMPTY_CLASS_H
#define EMPTY_CLASS_H

// =======================================================================================
// Class variable initialization

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
  EmptyClass(const EmptyClass&);                 // Prevent copy-construction
  EmptyClass& operator=(const EmptyClass&);      // Prevent assignment

};

#endif




