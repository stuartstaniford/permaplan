// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is for rendering sheds - either standalone buildings, or (with an open
// side) parts of larger buildings.

#include "Shed.h"


// =======================================================================================
// Constructor

Shed::Shed(void):
          VisualObject(false)
{
}


// =======================================================================================
// Destructor

Shed::~Shed(void)
{
}


// =======================================================================================
// Function to check the validity of some JSON in an OLDF file that purportedly represents
// a shed.

using namespace rapidjson;

bool Shed::validateOLDF(Value& shedJsonObject)
{
  return true;  
}


// =======================================================================================
