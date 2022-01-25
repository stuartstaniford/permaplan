// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -

#ifndef LOCKABLE_H
#define LOCKABLE_H
#include "Global.h"
#include <pthread.h>
#include <err.h>


// =======================================================================================
/// @brief Simple C++ wrapper around a pthread_mutex_lock
/// 
/// This class can either be used as a superclass for multiple inheritance, or as an 
/// object to be included in others.  If you inherit from this, you will get a mutex, and 
/// lock and unlock methods for it which directly form part of your class.  Or you can 
/// include one or Lockables in another class to provide locks.  Note that presently this 
/// class is completely empty if MULTI_THREADED_SIMULATION is not set.

class Lockable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Lockable(void);
  ~Lockable(void);

  /// @brief Locks the mutex.
  /// 
  /// Makes a call to pthread_mutex_lock on the private mutex variable.  Will abort the
  /// program if there's an error return.
  
  inline void lock(void)
   {
#ifdef MULTI_THREADED_SIMULATION
    if(pthread_mutex_lock(&mutex)) 
      err(-1, "Lock failure.\n");
#endif
   }

  /// @brief Unlocks the mutex.
  /// 
  /// Makes a call to pthread_mutex_unlock on the private mutex variable.  Will abort the
  /// program if there's an error return.

  inline void unlock(void)
   {
#ifdef MULTI_THREADED_SIMULATION
    if(pthread_mutex_unlock(&mutex)) err(-1, "Unlock failure.\n");
#endif
   }
    
protected:
  
  // Instance variables - protected
  pthread_mutex_t           mutex;

  // Member functions - private
  Lockable(const Lockable&);                 // Prevent copy-construction
  Lockable& operator=(const Lockable&);      // Prevent assignment

};


// =======================================================================================

#endif




