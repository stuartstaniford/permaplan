// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -

#ifndef TIMEVAL_H
#define TIMEVAL_H

#include <unistd.h>
#include <time.h>
#include <stdio.h>


// =======================================================================================
/// @brief Wrapper around Unix struct timeval to make manipulations a little easier.
/// 
/// See 'man gettimeofday' for details of the structure.

class Timeval: public timeval
{
 public:
  
  // Instance variables - public

  // Member functions - public
  Timeval(void);
  Timeval(FILE* file);
  ~Timeval();
  void    now();
  void    set(time_t tv_sec_in, suseconds_t tv_usec_in);
  bool    setFromJSON(char* buf);
  char*   ctimeString(void);
  void    ctimeDateOnly(char* buf);
  void    localtime(struct tm *result);
  float   floatYear(void);
  bool    writeBinaryToDisk(FILE* file);

  double operator-(const Timeval& t); // Difference between two times (in seconds)

 private:
  
  // Instance variables - private
  
  // Member functions - private
};


// =======================================================================================

#endif




