// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// A subclass of HttpLoadBalancer that is used for in the permaserv processing for 
// handling HTTP requests.  This subclass basically exists to make sure that requests 
// get handled by the correct HttpPermaServ instances that know how to interpret 
// the permaserv API.

#include "HttpLBPermaserv.h"
#include "HttpPermaServ.h"
#include "SolarDatabase.h"


// =======================================================================================
/// @brief Constructor

HttpLBPermaserv::HttpLBPermaserv(unsigned short servPort):
                                          HttpLoadBalancer(servPort)
{
  solarDatabase = new SolarDatabase;
  for(unsigned i=0; i<HTTP_THREAD_COUNT;i++)
    httpThreads[i] = (HttpServThread*) new HttpPermaServ(i, solarDatabase);
}


// =======================================================================================
/// @brief Destructor

HttpLBPermaserv::~HttpLBPermaserv(void)
{
}


// =======================================================================================
