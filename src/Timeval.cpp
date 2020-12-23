// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Wrapper around struct timeval to make manipulations a little easier.

#include "Timeval.h"
#include <sys/time.h>

// =======================================================================================
// Constructors

Timeval::Timeval(void)
{
}

// =======================================================================================
// Destructor

Timeval::~Timeval(void)
{
}


// =======================================================================================
// Wrapper for finding the current time.

void Timeval::now(void)
{
  gettimeofday(this, NULL);
}


// =======================================================================================
// Setting the inner timeval based on supplied arguments.

void Timeval::set(time_t tv_sec_in, suseconds_t tv_usec_in)
{
  tv_sec = tv_sec_in;
  tv_usec = tv_usec_in;
}


// =======================================================================================
// Return string version of time.  Note this loses fractions and includes \n - could
// do with something better.

char* Timeval::ctimeString(void)
{
  return ctime(&tv_sec);
}


// =======================================================================================
// Give the difference between two times (in seconds, as a double)

double Timeval::operator-(const Timeval& t)
{
  double diff;
  diff = (tv_sec - t.tv_sec);
  diff += tv_usec/1e6 ;
  diff -= t.tv_usec/1e6;
  return diff;
}


// =======================================================================================
