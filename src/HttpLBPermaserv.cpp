// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// A subclass of HttpLoadBalancer that is used for in the permaserv processing for 
// handling HTTP requests.  This subclass basically exists to make sure that requests 
// get handled by the correct HttpPermaServ instances that know how to interpret 
// the permaserv API.

#include "HttpLBPermaserv.h"
#include "HttpPermaServ.h"
#include "SolarDatabase.h"
#include "SoilDatabase.h"
#include "Logging.h"


// =======================================================================================
/// @brief Constructor

HttpLBPermaserv::HttpLBPermaserv(unsigned short servPort, time_t compTime):
                                          HttpLoadBalancer(servPort),
                                          compileTime(compTime)
{
  // Set up our component database objects
  solarDatabase = new SolarDatabase;
  soilDatabase = new SoilDatabase;
  LogPermaservOps("Initialization of databases complete.\n");
  LogFlush();
  
  // Run the threads to service requests
  for(unsigned i=0; i<HTTP_THREAD_COUNT;i++)
    httpThreads[i] = (TaskQueue*) new HttpPermaServ(i, solarDatabase, soilDatabase,
                                                          (HttpLoadBalancer*)this);
}


// =======================================================================================
/// @brief Destructor

HttpLBPermaserv::~HttpLBPermaserv(void)
{
}


// =======================================================================================
