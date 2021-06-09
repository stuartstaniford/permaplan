// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class has a basic load balancer which can accept connections and then hand
// them off for processing via TaskQueues to HttpDebug which does the work of handling
// an individual connection.  This allows multiple clients to talk to the HTTP interface.

#include "HttpLoadBalancer.h"

// =======================================================================================
// Constructor

HttpLoadBalancer::HttpLoadBalancer(void)
{
}


// =======================================================================================
// Destructor

HttpLoadBalancer::~HttpLoadBalancer(void)
{
}


// =======================================================================================
