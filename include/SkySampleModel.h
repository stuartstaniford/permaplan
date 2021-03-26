// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class encapsulates what is needed to sample the sky from any given location in order
// to estimate photosynthetic availability over the growing season.  It provides a method
// to come up with a series of directions, and a weight in each of those directions if a
// ray can in fact reach the sky in that direction.

// Useful resources:
// https://www.nrel.gov/gis/solar.html
// https://globalsolaratlas.info/download/world

// Global Horizontal Irradiance is the total amount of shortwave radiation received
// from above by a horizontal surface. This value is of particular interest to
// photovoltaic installations and includes both Direct Normal Irradiance (DNI) and
// Diffuse Horizontal Irradiance (DIF).

// Direct Normal Irradiance is the amount of solar radiation received per unit area
// by a surface that is always held perpendicular (or normal) to the rays that come
// in a straight line from the direction of the sun at its current position in the sky

// Diffuse Horizontal Irradiance is the amount of radiation received per unit area
// by a surface (not subject to any shade or shadow) that does not arrive on a direct
// path from the sun, but has been scattered by molecules and particles in the
// atmosphere and comes equally from all directions.

// The main useful output of this class is the 'samples' array, which is an array
// of vec4.  The first three values are a direction to sample in, and the fourth
// component is the growing season total irradiance from that direction, in kWH/m^2.

#ifndef SKY_SAMPLE_MODEL_H
#define SKY_SAMPLE_MODEL_H

#define SKY_SAMPLES 64

#include <cglm/cglm.h>

// =======================================================================================
// Class variable initialization


class SkySampleModel
{
public:
  
  // Instance variables - public
  vec4 samples[SKY_SAMPLES];
  
  // Member functions - public
  SkySampleModel(float lat);
  ~SkySampleModel(void);
  
private:
  
  // Instance variables - private
  float   latitude;         // in degrees
  float   gHI[12];          //global horizontal irradiance - kWH/m^2/day
  float   dNI[12];          //direct normal irradiance - kWH/m^2/day
  float   dFI[12];          //diffuse horizontal irradiance - kWH/m^2/day


  // Member functions - private
  SkySampleModel(const SkySampleModel&);                 // Prevent copy-construction
  SkySampleModel& operator=(const SkySampleModel&);      // Prevent assignment
};

#endif




