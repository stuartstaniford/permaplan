// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class has a basic load balancer which can accept connections and then hand
// them off for processing via TaskQueues to HttpDebug which does the work of handling
// an individual connection.  This allows multiple clients to talk to the HTTP interface.

#ifndef HTTP_LOAD_BALANCER_H
#define HTTP_LOAD_BALANCER_H

// =======================================================================================
// Class variable initialization

class HttpLoadBalancer
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpLoadBalancer(void);
  ~HttpLoadBalancer(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  HttpLoadBalancer(const HttpLoadBalancer&);                 // Prevent copy-construction
  HttpLoadBalancer& operator=(const HttpLoadBalancer&);      // Prevent assignment

};

#endif




