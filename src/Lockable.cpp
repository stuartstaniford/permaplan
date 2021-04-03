// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is a superclass for multiple inheritance.  If you inherit from this, 
// you will get a mutex and lock and unlock methods for it

#include "Lockable.h"

// =======================================================================================
// Constructor

Lockable::Lockable(void)
{
#ifdef MULTI_THREADED_SIMULATION
  pthread_mutex_init(&mutex, NULL);
#endif
}


// =======================================================================================
// Destructor

Lockable::~Lockable(void)
{
#ifdef MULTI_THREADED_SIMULATION
  pthread_mutex_destroy(&mutex);
#endif
}


// =======================================================================================
