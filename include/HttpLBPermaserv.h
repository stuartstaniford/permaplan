// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef HTTP_LB_PERMASERV_H
#define HTTP_LB_PERMASERV_H

#include "HttpLoadBalancer.h"
#include <time.h>


// =======================================================================================
// Forward declarations

class SolarDatabase;
class HttpPermaServ;


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
  HttpLBPermaserv(unsigned short servPort, time_t compTime);
  ~HttpLBPermaserv(void);
  
private:
  
  // Instance variables - private
  SolarDatabase*  solarDatabase;
  time_t          compileTime;
  
  // Member functions - private
  /// @brief Prevent copy-construction.
  HttpLBPermaserv(const HttpLBPermaserv&);       
  /// @brief Prevent assignment.
  HttpLBPermaserv& operator=(const HttpLBPermaserv&);      
};


// =======================================================================================

#endif




