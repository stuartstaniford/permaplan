// Copyright Staniford Systems.  All Rights Reserved.  Apr 2021 -
// This class is a superclass for multiple inheritance.  If you inherit from this, 
// you will get a mutex and lock and unlock methods for it

#include "Lockable.h"


// =======================================================================================
/// @brief Constructor initializes the mutex.
/// 
/// Makes a call to pthread_mutex_init on the private mutex variable. 

Lockable::Lockable(void)
{
#ifdef MULTI_THREADED_SIMULATION
  pthread_mutex_init(&mutex, NULL);
#endif
}


// =======================================================================================
/// @brief Destructor destroyes the mutex.
/// 
/// Makes a call to pthread_mutex_destroy on the private mutex variable. 

Lockable::~Lockable(void)
{
#ifdef MULTI_THREADED_SIMULATION
  pthread_mutex_destroy(&mutex);
#endif
}


// =======================================================================================
