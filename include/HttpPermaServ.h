// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -

#ifndef HTTP_PERMASERV_H
#define HTTP_PERMASERV_H

#include "HttpServThread.h"


// =======================================================================================
// Forward declarations

class SolarDatabase;


// =======================================================================================
/// @brief Http processing for requests to the permaserv server. 
///
/// Permaserv provides information from large geospatial databases to permaplan 
/// instances.  This class performs HTTP request processing for it (and is a subclass 
/// HttpServThread, which interfaces with the general Http processing system).

class HttpPermaServ: public HttpServThread
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  HttpPermaServ(unsigned index, SolarDatabase* solarD);
  ~HttpPermaServ(void);
  
private:
  
  // Instance variables - private
  SolarDatabase* solarDatabase;
  
  // Member functions - private
  bool  processRequestHeader(void);
  bool  indexPage(void);

  /// @brief Prevent copy-construction.
  HttpPermaServ(const HttpPermaServ&);       
  /// @brief Prevent assignment.
  HttpPermaServ& operator=(const HttpPermaServ&);      
};


// =======================================================================================

#endif




