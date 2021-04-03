// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is a superclass for multiple inheritance.  If you inherit from this, 
// you will get a mutex and lock and unlock methods for it

#ifndef LOCKABLE_H
#define LOCKABLE_H

// =======================================================================================
// Class variable initialization

class Lockable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Lockable(void);
  ~Lockable(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  Lockable(const Lockable&);                 // Prevent copy-construction
  Lockable& operator=(const Lockable&);      // Prevent assignment

};

#endif




