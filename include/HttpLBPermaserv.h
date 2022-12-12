// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef HTTP_LB_PERMASERV_H
#define HTTP_LB_PERMASERV_H

#include "HttpLoadBalancer.h"
#include "HttpPageSet.h"
#include <time.h>

// Flags which influence the operation of permaserv

#define PERMASERV_NO_SOLAR      0x00000001
#define PERMASERV_CLIMATE_FILES 0x00000002


// =======================================================================================
// Forward declarations

class SolarDatabase;
class SoilDatabase;
class ClimateDatabase;
class HttpPermaServ;


// =======================================================================================
/// @brief A mostly-POD class used to keep track of all parameters that affect permaserv
/// operations.

class PermaservParams
{
  public:
  
  time_t          compileTime;
  unsigned        flags;
  float           climateFileSpacing;
  unsigned short  servPort;

  PermaservParams(unsigned short port, unsigned flagsIn, float spacing);
};


// =======================================================================================
/// @brief A subclass of HttpLoadBalancer that is used in permaserv processing
/// for handling HTTP requests.
///
/// This subclass basically exists to make sure that requests get handled by the correct
/// HttpPermaServ instances that know how to interpret the permaserv API. 

class HttpLBPermaserv: public HttpLoadBalancer
{
  friend HttpPermaServ; // required for access to compileTime
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpLBPermaserv(PermaservParams& permaservParams);
  ~HttpLBPermaserv(void);
  void initializeScriptPages(void);
  
private:
  
  // Instance variables - private
  SolarDatabase*    solarDatabase;
  SoilDatabase*     soilDatabase;
  ClimateDatabase*  climateDatabase;
  PermaservParams&  params;
  HttpPageSet       scriptPages;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpLBPermaserv(const HttpLBPermaserv&);       
  /// @brief Prevent assignment.
  HttpLBPermaserv& operator=(const HttpLBPermaserv&);      
};


// =======================================================================================

#endif




