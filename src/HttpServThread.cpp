// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// This contains general functionality allowing it to be used by the system of 
// HTTPLoadBalancer.  This shouldn't be instantiated directly, but rather should be 
// subclassed, and the subclass should handle the specific requests of a given server
// (eg see HttpDebug as the original subclass of this).

#include "HttpServThread.h"


// =======================================================================================
/// @brief Constructor

HttpServThread::HttpServThread(unsigned index):
                                    TaskQueue(index)
{
}


// =======================================================================================
/// @brief Destructor

HttpServThread::~HttpServThread(void)
{
}


// =======================================================================================
