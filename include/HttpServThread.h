// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef HTTP_SERV_THREAD_H
#define HTTP_SERV_THREAD_H

#include "TaskQueue.h"

// =======================================================================================
/// @brief Superclass for any thread that is going to handle the specifics of an HTTP
/// server.
///
/// This contains general functionality allowing it to be used by the system of 
/// HTTPLoadBalancer.  This shouldn't be instantiated directly, but rather should be 
/// subclassed, and the subclass should handle the specific requests of a given server
/// (eg see HttpDebug as the original subclass of this).

class HttpServThread: public TaskQueue
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpServThread(unsigned index);
  ~HttpServThread(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpServThread(const HttpServThread&);       
  /// @brief Prevent assignment.
  HttpServThread& operator=(const HttpServThread&);      
};


// =======================================================================================

#endif




