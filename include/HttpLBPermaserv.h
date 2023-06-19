// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef HTTP_LB_PERMASERV_H
#define HTTP_LB_PERMASERV_H

#include "HttpLoadBalancer.h"
#include "HttpPageSet.h"
#include <time.h>

// Flags which influence the operation of permaserv

#define PERMASERV_NO_SOLAR      0x00000001
#define PERMASERV_CLIMATE_FILES 0x00000002
#define PERMASERV_NO_CLIMATE    0x00000004
#define PERMASERV_NO_USERS      0x00000008
#define PERMASERV_NO_OLDFSERV   0x00000010
#define PERMASERV_NO_TREES      0x00000020


// =======================================================================================
// Forward declarations

class SolarDatabase;
class SoilDatabase;
class ClimateDatabase;
class iTreeList;
class HttpPermaServ;
class PmodServer;


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
  friend HttpPermaServ; // required for access to compileTime, scripts, etc
  
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpLBPermaserv(PermaservParams& permaservParams);
  ~HttpLBPermaserv(void);
  void initializeScriptPages(void);
  void initializeCSSPages(void);
  
private:
  
  // Instance variables - private
  SolarDatabase*    solarDatabase;
  SoilDatabase*     soilDatabase;
  ClimateDatabase*  climateDatabase;
  iTreeList*        treeList;
  PmodServer*       pmodServer;
  PermaservParams&  params;
  HttpPageSet*      scriptPages;
  HttpPageSet*      cssPages;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpLBPermaserv(const HttpLBPermaserv&);       
  /// @brief Prevent assignment.
  HttpLBPermaserv& operator=(const HttpLBPermaserv&);      
};


// =======================================================================================

#endif




