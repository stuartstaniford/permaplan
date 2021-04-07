// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class encapsulates what is needed to sample the sky from any given location in order
// to estimate photosynthetic availability over the growing season.  It provides a method
// to come up with a series of directions, and a weight in each of those directions if a
// ray can in fact reach the sky in that direction.

// Useful resources:
// https://www.nrel.gov/gis/solar.html
// https://globalsolaratlas.info/download/world
// https://www.accuracyproject.org/w-FreezeFrost.html

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

// Growing season start is here taken to run from four weeks before last frost to
// two weeks after first frost.  This is rather imprecise and in particular in reality
// is very species dependent - things like grass/conifers that have leaves ready to
// go can have a longer season than deciduous plants.  Meanwhile, non-hardy plants
// will stop dead as soon as there's a frost.  It is assumed that no light is put
// to use outside of the growing season.

// The main useful output of this class is the 'samples' array, which is an array
// of vec4.  The first three values are a direction to sample in, and the fourth
// component is the growing season total irradiance from that direction, in kWH/m^2.

#ifndef SKY_SAMPLE_MODEL_H
#define SKY_SAMPLE_MODEL_H

#define SKY_SAMPLES 64        //
#define EARTH_TILT  23.44     // degrees

#define TREEi_SHADES_TREEj 0x00100000
#define TREEj_SHADES_TREEi 0x00200000
#define TREES_CLUSTER      0x00400000
#define TREES_INVALID      0x00800000

#include <cglm/cglm.h>
#include "BoundingBox.h"
#include "HttpDebug.h"

// =======================================================================================
// Class variable initialization


class SkySampleModel
{
public:
  
  // Instance variables - public
  vec4 samples[SKY_SAMPLES];

private:
  
  static SkySampleModel* theSingleton;

public:
  
  // Member functions - public
  SkySampleModel(float lat);
  ~SkySampleModel(void);
  void setSamples(void);
  unsigned treesInteract(BoundingBox* B1, BoundingBox* B2);
  bool treesCluster(BoundingBox* B1, BoundingBox* B2);
  bool oneSampleRow(HttpDebug* serv, int i);
  bool diagnosticHTML(HttpDebug* serv);

  static SkySampleModel& getSkySampleModel(void) // Get the singleton instance
  {
   return *theSingleton;
  }

private:
  
  // Instance variables - private
  float   latitude;              // in degrees
  float   gHI[12];               // global horizontal irradiance - kWH/m^2/day
  float   dNI[12];               // direct normal irradiance - kWH/m^2/day
  float   dFI[12];               // diffuse horizontal irradiance - kWH/m^2/day
  float   directElevations[5];   // the elevation ang
  int     seasonStart;           // day of the year growing season starts
  int     seasonEnd;             // ditto day it ends.


    // Member functions - private
  float declination(float dayOfYear);

  SkySampleModel(const SkySampleModel&);                 // Prevent copy-construction
  SkySampleModel& operator=(const SkySampleModel&);      // Prevent assignment
};

#endif




