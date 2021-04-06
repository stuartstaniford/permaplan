// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is a superclass for multiple inheritance.  If you inherit from this, 
// you will get a mutex and lock and unlock methods for it

#ifndef LOCKABLE_H
#define LOCKABLE_H
#include "Global.h"
#include <pthread.h>
#include <err.h>

// =======================================================================================
// Class variable initialization

class Lockable
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  Lockable(void);
  ~Lockable(void);

  inline void lock(void)
   {
#ifdef MULTI_THREADED_SIMULATION
    if(pthread_mutex_lock(&mutex)) err(-1, "Lock failure.\n");
#endif
   }
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

#endif




