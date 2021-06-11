// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class has a basic load balancer which can accept connections and then hand
// them off for processing via TaskQueues to HttpDebug which does the work of handling
// an individual connection.  This allows multiple clients to talk to the HTTP interface.

#include "HttpLoadBalancer.h"
#include "HttpDebug.h"
#include "Logging.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SA struct sockaddr

// =======================================================================================
// Constructor.  Create the socket and bind to the port in the constructor

HttpLoadBalancer::HttpLoadBalancer(unsigned short servPort, Scene& S, MenuInterface& imgMenu):
                                    scene(S),
                                    menuInterface(imgMenu),
                                    shutDownNow(false),
                                    port(servPort)
{
  // Get a socket
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err(-1, "Couldn't create socket in __func__\n");

  // Set up our address and port
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  // Bind the socket to the port
  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
    err(-1, "Couldn't bind socket on port %u in __func__\n", port);

  // Make our socket a server that will listen
  if ((listen(sockfd, 6)) != 0)
    err(-1, "Listen failed on socket %d in __func__\n", sockfd);

  HttpDebug** httpThreads = new HttpDebug*[HTTP_THREAD_COUNT];  
  for(unsigned i=0; i<HTTP_THREAD_COUNT;i++)
    httpThreads[i] = new HttpDebug(scene, menuInterface, i);
  servFarm = new TaskQueueFarm(HTTP_THREAD_COUNT, (TaskQueue**)httpThreads);
}


// =======================================================================================
// Destructor

HttpLoadBalancer::~HttpLoadBalancer(void)
{
  close(sockfd);
}


// =======================================================================================
// C function to pass to TaskQueue.

void processOneConnection(void* arg, TaskQueue* T)
{
  HttpDebug* http = (HttpDebug*)T;
  HttpLBWorkUnit* unit = (HttpLBWorkUnit*)arg;
  int connfd = unit->fileDescriptor;
  http->processOneHTTP1_1(connfd);
  close(connfd);
  
  unit->servFarm->notifyTaskDone();   
  delete unit;
}


// =======================================================================================
// Loop sitting on the socket accepting connections, process requests and reply to them.
// Arguments and return type are set up to be used in pthread_create

void* HttpLoadBalancer::processConnections(void)
{
  while(1)
   {
    // Accept a connection
    unsigned len = sizeof(cliaddr);
    if((connfd = accept(sockfd, (SA*)&cliaddr, &len)) < 0)
     {
      if(shutDownNow)
        break;
      else
        err(-1, "Accept failed on socket %d in __func__\n", sockfd);
     }
    LogHTTPDetails("Accepted connection from client on port %u.\n", cliaddr.sin_port);
    
    HttpLBWorkUnit* unit = new HttpLBWorkUnit;
    unit->fileDescriptor = connfd;
    unit->servFarm = servFarm;
    
    servFarm->loadBalanceTask(processOneConnection, unit);
        
    if(shutDownNow)
      break;
   }
  
  LogCloseDown("HttpDebug server shutting down normally.\n");
  return NULL;
}


// =======================================================================================
