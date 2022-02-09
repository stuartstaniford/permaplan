// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class has a basic load balancer which can accept connections and then hand
// them off for processing via TaskQueues to HttpDebug which does the work of handling
// an individual connection.  This allows multiple clients to talk to the HTTP interface.

#include "HttpLoadBalancer.h"
#include "HttpDebug.h"
#include "TaskQueueFarm.h"
#include "Logging.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SA struct sockaddr


// =======================================================================================
/// @brief Constructor for a new load balancer farm.
/// 
/// Creates the socket, binds to the port, and fires up a TaskQueueFarm of HttpServThread
/// instances to handle future requests.
/// @param servPort The server port to open up
/// @param S A reference to the scene (pass through to the HttpDebug instances, which
/// will need to interact with it.
/// @param windowApp A reference to the GLFWApplication.

HttpLoadBalancer::HttpLoadBalancer(unsigned short servPort):
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

  httpThreads = new TaskQueue*[HTTP_THREAD_COUNT];  
  servFarm = new TaskQueueFarm(HTTP_THREAD_COUNT, (TaskQueue**)httpThreads, (char*)"httpFarm");
}


// =======================================================================================
/// @brief Destructor

HttpLoadBalancer::~HttpLoadBalancer(void)
{
  close(sockfd);
}


// =======================================================================================
/// @brief C function to pass to TaskQueue.

void processOneConnection(void* arg, TaskQueue* T)
{
  HttpServThread* http = (HttpServThread*)T;
  HttpLBWorkUnit* unit = (HttpLBWorkUnit*)arg;
  int connfd = unit->fileDescriptor;
  http->processOneHTTP1_1(connfd, unit->clientPort);
  close(connfd);
  
  unit->servFarm->notifyTaskDone();   
  delete unit;
}


// =======================================================================================
/// @brief Main connection processing loop.
/// 
/// Loop sitting on the socket accepting connections and handing them off to one of our
/// TaskQueue instances to process.

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
    LogHTTPLoadBalance("Accepted connection from client on port %u.\n", cliaddr.sin_port);
    
    HttpLBWorkUnit* unit  = new HttpLBWorkUnit;
    unit->fileDescriptor  = connfd;
    unit->servFarm        = servFarm;
    unit->clientPort      = cliaddr.sin_port;
    
    servFarm->loadBalanceTask(processOneConnection, unit);
        
    for(int i=0; i<HTTP_THREAD_COUNT; i++)
     {
      //fprintf(stderr, "Checking timeToDie on %d: %d.\n", i, httpThreads[i]->timeToDie);
      if(httpThreads[i]->timeToDie)
        shutDownNow = true;
     }
    if(shutDownNow)
      break;
   }
  
  LogCloseDown("HttpDebug server shutting down normally.\n");
  return NULL;
}


// =======================================================================================
