// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// A subclass of HttpLoadBalancer that is used for in the permaserv processing for 
// handling HTTP requests.  This subclass basically exists to make sure that requests 
// get handled by the correct HttpPermaServ instances that know how to interpret 
// the permaserv API.

#include "HttpLBPermaserv.h"
#include "HttpPermaServ.h"
#include "SolarDatabase.h"
#include "SoilDatabase.h"
#include "ClimateDatabase.h"
#include "Logging.h"


// =======================================================================================
/// @brief Constructor

PermaservParams::PermaservParams(unsigned short port, unsigned flagsIn, float spacing):
                                            flags(flagsIn),
                                            climateFileSpacing(spacing),
                                            servPort(port)
{
  
}

// =======================================================================================
/// @brief Constructor

HttpLBPermaserv::HttpLBPermaserv(PermaservParams& permaservParams):
                                          HttpLoadBalancer(permaservParams.servPort),
                                          params(permaservParams)
{
  // Set up our component database objects
  if(params.flags & PERMASERV_NO_SOLAR)
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
  
  if(params.flags & PERMASERV_NO_CLIMATE)
   {
    climateDatabase = NULL;
    LogPermaservOps("Initializing without climate database.\n");
   }
  else if(params.flags & PERMASERV_CLIMATE_FILES)
   {
    climateDatabase = new ClimateDatabase(params.climateFileSpacing);
    LogPermaservOps("Full initialization of climate database complete.\n");
   }
  else
   {
    climateDatabase = new ClimateDatabase;
    LogPermaservOps("Basic initialization of climate database complete.\n");
   }
  
  initializeScriptPages();
  initializeCSSPages();
  
  // Run the threads to service requests
  for(unsigned i=0; i<HTTP_THREAD_COUNT;i++)
    httpThreads[i] = (TaskQueue*) new HttpPermaServ(i, solarDatabase, soilDatabase, 
                                                  climateDatabase, (HttpLoadBalancer*)this);
}


// =======================================================================================
/// @brief Destructor

HttpLBPermaserv::~HttpLBPermaserv(void)
{
}


// =======================================================================================
/// @brief Function to set up static script pages that we serve.
/// 
/// Any script that we want to serve should be included in here and set to NULL.  It will
/// be loaded the first time it's requested.

void HttpLBPermaserv::initializeScriptPages(void)
{
  scriptPages = new HttpPageSet((char*)"scripts/", (char*)"text/javascript");
  
  scriptPages->insert({"test.js", NULL});
}


// =======================================================================================
/// @brief Function to set up static css pages that we serve.
/// 
/// Any CSS file that we want to serve should be included in here and set to NULL.  It will
/// be loaded the first time it's requested.

void HttpLBPermaserv::initializeCSSPages(void)
{
  cssPages = new HttpPageSet((char*)"css/", (char*)"text/css");
  
  cssPages->insert({"permaplan.css", NULL});
}


// =======================================================================================
