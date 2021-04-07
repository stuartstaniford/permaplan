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

  setSamples();
}


// =======================================================================================
// Destructor

SkySampleModel::~SkySampleModel(void)
{
}


// =======================================================================================
// Function that gives us a new random distribution in the samples (which we do 
// periodically throughout the simulation to increase overall sampling power, rather than 
// using the same ones year after simulated year.

void SkySampleModel::setSamples(void)
{
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
    
    // get rid of things that will cause numerical blow-ups.
    if(fabs(point[1]) < EPSILON)
      continue;
      
    glm_vec3_scale_as(point, 1.0f, samples[pointsAchieved]);
    samples[pointsAchieved][3] = 3.14159f;
    
    pointsAchieved++;
   }
}


// =======================================================================================
// Function to determine, based on bounding box positions, whether tree1 and tree2
// might need to consider their mutual interaction in terms of shading one another.

// Also to determine, based on bounding boxes whether tree1 and tree2
// are so close to each other that for purposes of shading, they should be considered
// as a single cluster (a "copse").  Can also be applied to clusters of trees.

// These two somewhat distinct concepts are combined in this function to avoid doing
// all the same calculations twice (eg the sqrtf() in the distance between the trees).

// Returns an unsigned with bit fields set (defined in SkySampleModel.h.  
// Can also be applied to clusters of trees.

//XX in general this is crude and needs refinement over time.

float minimalAngleAboveHorizon = atanf(22.5f/180.0f*M_PI);
float minDistOverHeight = 0.33f;

unsigned SkySampleModel::treesInteract(BoundingBox* B1, BoundingBox* B2)
{
  unsigned retVal = 0x00000000;
  float xDist = (B1->lower[0] + B1->upper[0] - B2->lower[0] - B2->upper[0])/2.0f;
  float yDist = (B1->lower[1] + B1->upper[1] - B2->lower[1] - B2->upper[1])/2.0f;
  float horizDist = sqrtf(xDist*xDist + yDist*yDist); // Pythagoras
               
  //XX would be better to model the crown bottom here 
  if((B2->upper[2] - B1->lower[2])/horizDist > minimalAngleAboveHorizon)
    //B2 might shade B1
    retVal |= TREEi_SHADES_TREEj;

  if((B1->upper[2] - B2->lower[2])/horizDist > minimalAngleAboveHorizon)
    //B1 might shade B1
    retVal |= TREEj_SHADES_TREEi;  

  float minHeight = B1->upper[2] - B1->lower[2];
  if(B1->upper[2] - B1->lower[2] < minHeight)
    minHeight = B1->upper[2] - B1->lower[2];        //XX not going to work across a cliff
  
  if(horizDist/minHeight < minDistOverHeight)
    retVal |= TREES_CLUSTER;  

  LogTreeGraph("Tree horiz dist %.1f, B2-B1 %.1f, B1-B2, %.1f, minHeight, %.1f.\n",
               horizDist, B2->upper[2] - B1->lower[2], B1->upper[2] - B2->lower[2], minHeight);
  
  return retVal;
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
// Helper function for diagnosticHTML to do one row of main tables

bool SkySampleModel::oneSampleRow(HttpDebug* serv, int i)
{
  vec4& sample = samples[i];
  float azimuth = atanf(sample[0]/sample[1])*180.0f/M_PI;
  float elevation = atanf(sample[2]
                          /sqrt(sample[0]*sample[0] + sample[1]*sample[1]))*180.0f/M_PI;
    
  httPrintf("<tr><td>%d</td><td>%.1f</td><td>%.1f</td><td>%.1f</td>",
            i, azimuth, elevation, sample[3]);
  httPrintf("<td>%.3f</td><td>%.3f</td><td>%.3f</td></tr>\n", sample[0], sample[1], sample[2]);
  return true;
}


// =======================================================================================
// Provide a diagnostic page with a table about all the current sky samples

bool SkySampleModel::diagnosticHTML(HttpDebug* serv)
{
  unless(serv->startResponsePage("Sky Samples"))
    return false;
  
  httPrintf("<center>\n");
  
  // The direct (sunlit) samples
  httPrintf("<h2>Direct/Sunlit sample vectors</h2>\n");
  unless(serv->startTable())
    return false;
  httPrintf("<tr><th>Sample</th><th>Azimuth</th><th>Elevation</th><th>Photon Flux</th><th>X</th><th>Y</th><th>Z</th></tr>\n");
  for(int i=0; i < SKY_SAMPLES/2; i++)
    unless(oneSampleRow(serv, i))
      return false;
  httPrintf("</table><hr>\n");

  // The indirect (scattered/overcast) samples
  httPrintf("<h2>Indirect (scattered/overcast) sample vectors</h2>\n");
  unless(serv->startTable())
    return false;
  httPrintf("<tr><th>Sample</th><th>Azimuth</th><th>Elevation</th><th>Photon Flux</th><th>X</th><th>Y</th><th>Z</th></tr>\n");
  for(int i=SKY_SAMPLES/2; i < SKY_SAMPLES; i++)
    unless(oneSampleRow(serv, i))
      return false;
  
  httPrintf("</table></center><hr>\n");
  if(!serv->endResponsePage())
    return false;
  return true;
}


// =======================================================================================
