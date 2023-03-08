// Copyright Staniford Systems.  All Rights Reserved.  March 2023 -

#ifndef TIMEOUT_MAP_H
#define TIMEOUT_MAP_H

#include "Global.h"
#include "Timeval.h"
#include "Lockable.h"
#include <unordered_map>


// =======================================================================================
/// @brief An enum for whether a given Timeoutable was present, expired, or not present

enum EntryStatus
{
  EntryNotPresent,
  EntryPresent,
  EntryExpired
};


// =======================================================================================
/// @brief Implements an unordered_map of things that need to have a timeout.

class Timeoutable: public Lockable
{
public:
  // Member functions - public
  Timeoutable(void);
  ~Timeoutable(void);
  
private:
  
  // Instance variables - private
  Timeval lastAccess;  

  // Member functions - private
  PreventAssignAndCopyConstructor(Timeoutable);
};


// =======================================================================================
/// @brief Implements an unordered_map of things that need to have a timeout.
///
/// More details of the class needed here.

template<typename Key>
class TimeoutMap: private std::unordered_map<Key, Timeoutable*>
{
public:
  
  // Instance variables - public
    
private:
  
  // Instance variables - private
  
  // Member functions - private
  PreventAssignAndCopyConstructor(TimeoutMap);

protected:

  // Instance variables - protected
  Lockable  mapLock;

public:
  
  // Member functions - public
  // See below for function definitions of the template class
  
// =======================================================================================
/// @brief Constructor

TimeoutMap(void)
{
}


// =======================================================================================
/// @brief Destructor

~TimeoutMap(void)
{
}


// =======================================================================================
/// @brief Function to find a particular entry based on key.
/// @returns A pointer to the Timeoutable entry, which will be returned locked.  Caller
/// is responsible for unlocking the entry lock.

Timeoutable* findEntry(Key key, EntryStatus& status)
{
  mapLock.lock();
  unless((std::unordered_map<Key, Timeoutable*>::count(key)))
   {
    status = EntryNotPresent;
    mapLock.unlock();
    return NULL;
   }
  
  (*this)[key]->lock();
  mapLock.unlock();
  return (*this)[key];
}

  
// =======================================================================================

}; // End of class definition


#endif




