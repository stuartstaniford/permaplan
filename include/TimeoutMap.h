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
  void updateAccessToNow(void);
  
  // Instance variables - public
  Timeval lastAccess;  

private:
  
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
  unsigned timeoutSecs;
  
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

TimeoutMap(unsigned timeout): timeoutSecs(timeout)
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
  
  // Check if the entry is missing altogether
  unless((std::unordered_map<Key, Timeoutable*>::count(key)))
   {
    status = EntryNotPresent;
    mapLock.unlock();
    return nullptr;
   }
  
  // Get the current time
  Timeval current;
  current.now();
  
  // Check if the entry has expired
  unless(current - (*this)[key]->lastAccess < timeoutSecs)
   {
    status = EntryExpired;
    mapLock.unlock();
    return nullptr;
   }
  
  // Handle a successful search for the entry
  (*this)[key]->lock();
  (*this)[key]->updateAccessToNow();
  status = EntryPresent;
  mapLock.unlock();
  return (*this)[key]; // Note, is still locked!!
}

  
// =======================================================================================
/// @brief Function to add a particular entry to the map.
/// 
/// @param key The Key which is going to be used to store the entry
/// @param entry A pointer to the Timeoutable entry, which should be unlocked (but no other
/// thread should be able to reach it until after it's stored here).
/// @returns true if successfully added.  False if it would be a duplicate.
  
  bool addEntry(Key key, Timeoutable* entry)
  {
    mapLock.lock();
    if((std::unordered_map<Key, Timeoutable*>::count(key)))
     {
      mapLock.unlock();
      return false;
     }
    
   entry->updateAccessToNow();
   (*this)[key] = entry;
   mapLock.unlock();
   return true;
  }

    
// =======================================================================================

}; // End of class definition


#endif




