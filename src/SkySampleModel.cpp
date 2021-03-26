// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class encapsulates what is needed to sample the sky from any given location
// in order to estimate photosynthetic availability over the growing season.  It
// provides a method to come up with a series of directions, and a weight in each of
// those directions if a ray can in fact reach the sky in that direction.

#include "SkySampleModel.h"

// Useful constants.

// =======================================================================================
// Constructor

SkySampleModel::SkySampleModel(float lat):
                                  latitude(lat)
{
  // Currently hard-coded Ithaca values (approx).
  // XX need to actually look up based on latitude in available dataset.
  
}


// =======================================================================================
// Destructor

SkySampleModel::~SkySampleModel(void)
{
}


// =======================================================================================
