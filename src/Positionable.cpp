// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class can be inherited from for things that have a position and provides some
// standard methods for adjusting the position (eg menu subpanels and on-screen arrows.
// In this class, the [0] (x) and [1] (y) directions have the obvious absolute meaning,
// but the [2] (z) direction is interpreted relative to the ground surface.

#include "Positionable.h"
#include "HttpDebug.h"

// =======================================================================================
/// @brief Destructor

Positionable::~Positionable(void)
{
}


// =======================================================================================
/// @brief Create a single table cell about this position.
/// @returns True if the object was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool Positionable::httPrintPositionCell(HttpDebug* serv)
{
  httPrintf("<td><b>Position:</b> [%.1f, %.1f, %.1f]</td></tr>\n",
                                          position[0], position[1], position[2]);
  return true;
}


// =======================================================================================
