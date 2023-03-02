// Copyright Staniford Systems.  All Rights Reserved.  Sep 2020 -
// Wrapper around struct timeval to make manipulations a little easier.

#include "Timeval.h"
#include "Global.h"
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>


// =======================================================================================
/// @brief Constructor

Timeval::Timeval(void)
{
}


// =======================================================================================
/// @brief Constructor reading the value from a binary file
///
/// @param file The stdio FILE* pointer to the open file to read from

Timeval::Timeval(FILE* file)
{
}


// =======================================================================================
/// @brief Destructor

Timeval::~Timeval(void)
{
}


// =======================================================================================
/// @brief Write out value to a binary file.
///
/// @param file The stdio FILE* pointer to the open file to write to

bool Timeval::writeBinaryToDisk(FILE* file)
{
  return true;
}


// =======================================================================================
/// @brief Set us to the current time.

void Timeval::now(void)
{
  gettimeofday(this, NULL);
}


// =======================================================================================
/// @brief Set the inner timeval based on supplied arguments.
/// 
/// See 'man gettimeofday' for details of the structure.
/// @param tv_sec_in The number of seconds since the unix epoch
/// @param tv_usec_in The number of microseconds (to represent the fractional part of 
/// the time).

void Timeval::set(time_t tv_sec_in, suseconds_t tv_usec_in)
{
  tv_sec = tv_sec_in;
  tv_usec = tv_usec_in;
}


// =======================================================================================
/// @brief Setting the inner timeval based on a JSON array in a character buffer.  
/// @returns false if buffer contents are not valid, true otherwise.  
/// @param buf A C-string pointer to the section of JSON to be parsed (which should look 
/// like [<secs>, <usecs>]).
/// @todo This function has not been used/tested yet.

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
/// @brief Return string version of time. 
/// @returns A pointer to a C-string holding the result of ctime (see 'man ctime')
/// @todo Note this loses fractions and includes \n - could do with something better.

char* Timeval::ctimeString(void)
{
  return ctime(&tv_sec);
}


// =======================================================================================
/// @brief Fill out a "struct tm" based on our current time.
/// 
/// See "man localtime" for details.
/// @param result A pointer to a struct tm to hold the result.

void Timeval::localtime(struct tm *result)
{
  localtime_r(&tv_sec, result);
}


// =======================================================================================
/// @brief Return our current time expressed as a fractional year.
/// @returns A floating point value expressing the time in years since the Unix epoch.

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
/// @brief Give the difference between two times (in seconds, as a double)

double Timeval::operator-(const Timeval& t)
{
  double diff;
  diff = (double)((long long)tv_sec - (long long)t.tv_sec);
  diff += tv_usec/1e6 ;
  diff -= t.tv_usec/1e6;
  return diff;
}


// =======================================================================================
