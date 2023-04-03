// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -

#ifndef HTTP_LOAD_BALANCER_H
#define HTTP_LOAD_BALANCER_H

#define HTTP_THREAD_COUNT 3

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>


// =======================================================================================
// Forward declarations

class Scene; 
class MenuInterface; 
class HttpDebug;
class TaskQueueFarm;
class TaskQueue;
class GLFWApplication;
class HttpServThread;
class UserSessionGroup;


// =======================================================================================
/// @brief Load balancer for HTTP debugging requests.
/// 
/// This class has a basic load balancer which can accept connections and then hand
/// them off for processing via TaskQueues to HttpDebug which does the work of 
/// handling an individual connection.  This allows multiple clients to talk to the 
/// HTTP interface.


class HttpLoadBalancer
{
public:
  
  // Instance variables - public
  bool                shutDownNow; // Reads/writes on a single bool should be atomic, no lock.
  
  // Member functions - public
  HttpLoadBalancer(unsigned short servPort, bool haveSessions = false);
  ~HttpLoadBalancer(void);
  void* processConnections(void);

protected:
  TaskQueue**  httpThreads;  // must be initialized by subclass that knows the 
                                  // real type of these
  UserSessionGroup* userSessions;
  
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


// =======================================================================================
/// @brief Plain old data class for a unit of work for the HttpLoadBalancer.

class HttpLBWorkUnit
{  
  public:
    TaskQueueFarm*  servFarm;
    int             fileDescriptor;
    unsigned short  clientPort;
};


// =======================================================================================

#endif




