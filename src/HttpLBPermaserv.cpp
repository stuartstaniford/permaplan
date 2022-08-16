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

HttpLBPermaserv::HttpLBPermaserv(unsigned short servPort, time_t compTime, 
                                                              unsigned initFlags):
                                          HttpLoadBalancer(servPort),
                                          compileTime(compTime),
                                          flags(initFlags)
{
  // Set up our component database objects
  if(flags & PERMASERV_NO_SOLAR)
   {
    solarDatabase = NULL;
    LogPermaservOps("Initializing without solar database.\n");
   }
  else
   {
    solarDatabase = new SolarDatabase;
    LogPermaservOps("Initialization of solar database complete.\n");
   }
  soilDatabase = new SoilDatabase;
  LogPermaservOps("Initialization of soil database complete.\n");
  
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
