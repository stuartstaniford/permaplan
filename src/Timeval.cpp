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
// Give the difference between two times (in seconds, as a double)

double Timeval::operator-(const Timeval& t)
{
  double diff;
  diff = (this->tv_sec - t.tv_sec);
  diff += this->tv_usec/1e6 ;
  diff -= t.tv_usec/1e6;
  return diff;
}


// =======================================================================================
