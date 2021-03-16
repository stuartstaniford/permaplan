// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Wrapper around struct timeval to make manipulations a little easier.

#include "Timeval.h"
#include "GlobalMacros.h"
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

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
// Setting the inner timeval based on a JSON array in a character buffer.  Returns false
// if buffer contents are not valid.  Should look like [<secs>, <usecs>].

// This function has not been used/tested yet.

bool Timeval::setFromJSON(char* buf)
{
  if(buf[0] != '[' || buf[1] == '\0')
    return false;
  
  // Read the first number with strtol and handle errors
  char* endPtr;
  tv_sec = strtol(buf+1, &endPtr, 10);
  if(*endPtr == '\0')
    return false;   // string ended before we got to comma
  if(endPtr == buf+1)
    return false;  // no valid digits in string
  
  //We are past the first number.  Now expecting comma, after possible white space
  while(*endPtr != '\0')
   {
    if(isWhiteSpace(*endPtr))
     {
      endPtr++;
      continue;
     }
    if(*endPtr == ',')
     {
      // We have correctly found the comma after the first number
      buf = endPtr + 1;
      if(buf[0] == '\0')
        return false;  // second number is empty
      tv_usec = strtol(buf, &endPtr, 10);
      if(*endPtr == '\0')
        return false;   // string ended before we got to ]
      if(endPtr == buf)
        return false;  // no valid digits in string

      // Now we have the second number, but we still check that there is a closing ']'
      while(*endPtr != '\0')
       {
        if(isWhiteSpace(*endPtr))
         {
          endPtr++;
          continue;
         }
        if(*endPtr == ']')
          // we are golden
          return true;
        return false; // weird character instead of ws or ]
       }
      return false; // string ended prematurely
     }
    else
      return false; // weird character instead of ws or ,
   }
  
  // if we get down here, string ended prematurely
  return false;
}


// =======================================================================================
// Return string version of time.  Note this loses fractions and includes \n - could
// do with something better.

char* Timeval::ctimeString(void)
{
  return ctime(&tv_sec);
}


// =======================================================================================
// Fill out a "struct tm" (see "man localtime") based on our current time.

void Timeval::localtime(struct tm *result)
{
  localtime_r(&tv_sec, result);
}


// =======================================================================================
// Return the time expressed as a fractional year.

float Timeval::floatYear(void)
{
  struct tm T;
  localtime_r(&tv_sec, &T);
  if(T.tm_year%4)
    // not leap year
    return 1900.0f + T.tm_year + (T.tm_yday + (T.tm_hour +
                            (T.tm_min + T.tm_sec/60.0f)/60.0f)/24.0f)/365.0f;
  else
    // leap year
    return 1900.0f + T.tm_year + (T.tm_yday + (T.tm_hour +
                            (T.tm_min + T.tm_sec/60.0f)/60.0f)/24.0f)/366.0f;
}


// =======================================================================================
// Give the difference between two times (in seconds, as a double)

double Timeval::operator-(const Timeval& t)
{
  double diff;
  diff = (double)((long long)tv_sec - (long long)t.tv_sec);
  diff += tv_usec/1e6 ;
  diff -= t.tv_usec/1e6;
  return diff;
}


// =======================================================================================
