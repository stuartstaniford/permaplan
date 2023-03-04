// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -

#ifndef TIMEOUT_MAP_H
#define TIMEOUT_MAP_H

#include "Global.h"
#include "Timeval.h"
#include <unordered_map>


// =======================================================================================
/// @brief Implements an unordered_map of things that need to have a timeout.

class Timable
{
  Timeval lastAccess;  
};


// =======================================================================================
/// @brief Implements an unordered_map of things that need to have a timeout.
///
/// More details of the class needed here.

template<typename Key>
class TimeoutMap: public std::unordered_map<Key, Timable*>
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  TimeoutMap(void);
  ~TimeoutMap(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(TimeoutMap);
};


// =======================================================================================

#endif




