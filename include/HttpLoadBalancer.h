// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class has a basic load balancer which can accept connections and then hand
// them off for processing via TaskQueues to HttpDebug which does the work of handling
// an individual connection.  This allows multiple clients to talk to the HTTP interface.

#ifndef HTTP_LOAD_BALANCER_H
#define HTTP_LOAD_BALANCER_H

#include "TaskQueueFarm.h"

#define HTTP_THREAD_COUNT 3

// =======================================================================================
// Class variable initialization

class Scene; 
class MenuInterface; 
class HttpDebug;

class HttpLoadBalancer
{
public:
  
  // Instance variables - public
  Scene&              scene;
  MenuInterface&      menuInterface;
  bool                shutDownNow; // Reads/writes on a single bool should be atomic, no lock.
  
  // Member functions - public
  HttpLoadBalancer(unsigned short servPort, Scene& S, MenuInterface& imgMenu);
  ~HttpLoadBalancer(void);
  void* processConnections(void);
  
private:
  
  // Instance variables - private
  TaskQueueFarm*      servFarm;
  struct sockaddr_in  servaddr;
  struct sockaddr_in  cliaddr;
  int                 sockfd;
  int                 connfd;
  unsigned short      port;
  
  // Member functions - private
  HttpLoadBalancer(const HttpLoadBalancer&);                 // Prevent copy-construction
  HttpLoadBalancer& operator=(const HttpLoadBalancer&);      // Prevent assignment
};

class HttpLBWorkUnit
{  
  public:
    TaskQueueFarm* servFarm;
    int fileDescriptor;
};


#endif




