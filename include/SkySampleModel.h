// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class encapsulates what is needed to sample the sky from any given location in order
// to estimate photosynthetic availability over the growing season.  It provides a method
// to come up with a series of directions, and a weight in each of those directions if a
// ray can in fact reach the sky in that direction.


#ifndef SKY_SAMPLE_MODEL_H
#define SKY_SAMPLE_MODEL_H

#include <cglm/cglm.h>

// =======================================================================================
// Class variable initialization


class SkySampleModel
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  SkySampleModel(void);
  ~SkySampleModel(void);
  
private:
  
  // Instance variables - private
  float   latitude;

  // Member functions - private
  SkySampleModel(const SkySampleModel&);                 // Prevent copy-construction
  SkySampleModel& operator=(const SkySampleModel&);      // Prevent assignment
};

#endif




