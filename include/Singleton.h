// Copyright Staniford Systems.  All Rights Reserved.  January 2023 -

#ifndef SINGLETON_H
#define SINGLETON_H


// =======================================================================================
/// @brief Superclass for any class that needs to implement the singleton pattern.
///
/// Any subclass of this needs to do the following:

class Singleton
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Singleton(void);
  ~Singleton(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  Singleton(const Singleton&);       
  /// @brief Prevent assignment.
  Singleton& operator=(const Singleton&);      
};


// =======================================================================================

#endif




