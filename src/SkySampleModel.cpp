// Copyright Staniford Systems.  All Rights Reserved.  Mar 2021 -
// This class encapsulates what is needed to sample the sky from any given location
// in order to estimate photosynthetic availability over the growing season.  It
// provides a method to come up with a series of directions, and a weight in each of
// those directions if a ray can in fact reach the sky in that direction.

// https://en.wikipedia.org/wiki/Position_of_the_Sun
// https://en.wikipedia.org/wiki/Solar_azimuth_angle
// https://en.wikipedia.org/wiki/Hour_angle

// For converting W/m^2 to photon fluxes:
//https://www.researchgate.net/post/Can-I-convert-PAR-photo-active-radiation-value-of-micro-mole-M2-S-to-Solar-radiation-in-Watt-m2/59ca6422217e201e2b23415f/citation/download

#include "Logging.h"
#include "SkySampleModel.h"

// Useful constants.

SkySampleModel* SkySampleModel::theSingleton = NULL;

// =======================================================================================
// Constructor

// Currently hard-coded Ithaca values (approx).
// XX need to actually look up based on latitude in a suitable dataset.

SkySampleModel::SkySampleModel(float lat):
                                  latitude(lat),
                                  gHI{1.0f, 1.5f, 3.5f, 4.5f, 5.6f, 5.8f,
                                      5.8f, 5.4f, 4.25f, 3.5f, 1.8f, 1.0f},
                                  dNI{3.1f, 3.2f, 4.2f, 4.7f, 4.7f, 5.0f,
                                      5.3f, 5.2f, 4.7f, 3.8f, 3.5f, 3.2f},
                                  seasonStart(105),
                                  seasonEnd(303)
{
  // Constructor should only be called once at startup.  Everyone else gets us via
  // getSkySampleModel()
  if(theSingleton)
    return;
  else
    theSingleton = this;

  // We use half the SKY_SAMPLES to estimate the effect of direct sunlight, and the
  // other half to estimate the effect of indirect (scattered) sunlight.
  
  // Deal with the DNI samples
  int seasonLength = seasonEnd - seasonStart;
  int day, i;
  for(day = seasonStart + seasonLength/10, i=0; i<5; day += seasonLength/5, i++)
   {
    directElevations[i] = 90.0f - latitude + declination(day);
    LogSkySampleInit("Elevations[%d] at day %d is %.1f.\n", i, day, directElevations[i]);
   }

  //XX still to do
  int pointsAchieved = SKY_SAMPLES/2;

  // Deal with non DNI samples
  // Strategy is to pick random points in the celestial sphere, filter them out
  // if they don't work, then project into directions.
  float sphereRadius = 1000.0f;
  vec3 point;
  while(pointsAchieved < SKY_SAMPLES)
   {
    // Generate a point in the upper half cube
    point[0] = (float)( (double)random()/(double)RAND_MAX*sphereRadius*2.0f - sphereRadius);
    point[1] = (float)( (double)random()/(double)RAND_MAX*sphereRadius*2.0f - sphereRadius);
    point[2] = (float)( (double)random()/(double)RAND_MAX*sphereRadius);

    // Get rid of ones that aren't in the upper half sphere
    if(point[0]*point[0] + point[1]*point[1] + point[2]*point[2] > sphereRadius*sphereRadius)
      continue;
    
    //XX need to finish up in here
    
    samples[pointsAchieved][0] = 1.0f;
    pointsAchieved++;
   }
}


// =======================================================================================
// Destructor

SkySampleModel::~SkySampleModel(void)
{
}

// =======================================================================================
// Function to return the declination angle of the sun based on day of year.
// See https://en.wikipedia.org/wiki/Position_of_the_Sun

float SkySampleModel::declination(float dayOfYear)
{
  return -asinf(0.39779*cosf(0.017202838*(dayOfYear+10.0f)
                          +0.033405602*sinf(0.017202838*(dayOfYear-2.0f))))/2/M_PI*360.0f;
  
}

// =======================================================================================
