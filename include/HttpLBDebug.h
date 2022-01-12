// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef HTTP_LB_DEBUG_H
#define HTTP_LB_DEBUG_H

#include "HttpLoadBalancer.h"


// =======================================================================================
/// @brief A subclass of HttpLoadBalancer that is specifically used for the debugging
/// interface in Permaplan itself.
///
/// This class has a basic load balancer which can accept connections and then hand
/// them off for processing via TaskQueues to HttpDebug which does the work of 
/// handling an individual connection.  This allows multiple clients to talk to the 
/// HTTP interface.

class HttpLBDebug: public HttpLoadBalancer
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpLBDebug(unsigned short servPort, Scene& S, GLFWApplication& winApp);
  ~HttpLBDebug(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpLBDebug(const HttpLBDebug&);       
  /// @brief Prevent assignment.
  HttpLBDebug& operator=(const HttpLBDebug&);      
};


// =======================================================================================

#endif




