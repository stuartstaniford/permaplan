// Copyright Staniford Systems.  All Rights Reserved.  July 2023 -
// Models a biological order.  Inherits from an unordered map of names to Families

#include "Order.h"
#include "HttpServThread.h"
#include "Logging.h"
#include <string.h>


// =======================================================================================
/// @brief Constructor

Order::Order(char* orderName)
{
  name = strdup(orderName);
}


// =======================================================================================
/// @brief Destructor

Order::~Order(void)
{
  free(name);
}


// =======================================================================================
