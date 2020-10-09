// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Wrapper around struct timeval to make manipulations a little easier.


#ifndef TIMEVAL_H
#define TIMEVAL_H

#include <unistd.h>

// =======================================================================================
// Class variable initialization


class Timeval: public timeval
{
 public:
  
  // Instance variables - public

  // Member functions - public
  Timeval(void);
  ~Timeval();
  void now();
  double operator-(const Timeval& t); // Difference between two times (in seconds)


 private:
  
  // Instance variables - private
  
  // Member functions - private
};

#endif




