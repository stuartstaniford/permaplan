// Copyright Staniford Systems.  All Rights Reserved.  January 2022 -
// A subclass of HttpLoadBalancer that is specifically used for the debugging
// interface in Permaplan itself.  This class has a basic load balancer which 
// can accept connections and then hand them off for processing via TaskQueues 
// to HttpDebug which does the work of handling an individual connection.  This 
// allows multiple clients to talk to the HTTP interface.

#include "HttpLBDebug.h"


// =======================================================================================
/// @brief Constructor

HttpLBDebug::HttpLBDebug(void)
{
}


// =======================================================================================
/// @brief Destructor

HttpLBDebug::~HttpLBDebug(void)
{
}


// =======================================================================================
