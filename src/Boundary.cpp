
// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for storing and working with the boundary of a plot of land, as in the OLDF
// boundary object.

#include "Boundary.h"
#include "PmodConfig.h"
#include "Logging.h"
#include "GlobalMacros.h"

// =======================================================================================
// Constructors

Boundary::Boundary()
{
}


// =======================================================================================
// Destructor

Boundary::~Boundary(void)
{
}


// =======================================================================================
// Function to set us up from an OLDF boundary object (assumed validated before being
// supplied to us (eg by validateBoundaries below)

using namespace rapidjson;

void Boundary::setFromOLDF(Value& boundaries)
{
  referencePoint[0] = boundaries["referencePoint"][0].GetFloat();
  referencePoint[1] = boundaries["referencePoint"][1].GetFloat();
  int N = boundaries["arcs"].Size();
  arcs.reserve(2*N);
  for(int i=0; i<N; i++)
   {
    arcs.push_back(boundaries["arcs"][i][0].GetFloat());
    arcs.push_back(boundaries["arcs"][i][1].GetFloat());
   }
}


// =======================================================================================
// Function to check the OLDF boundary reference point.

bool Boundary::validateReferencePoint(Value& boundaries)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  
  if(boundaries.HasMember("referencePoint") && boundaries["referencePoint"].IsArray())
   {
    Value& refPointArray = boundaries["referencePoint"];
    if(refPointArray.Size() == 2)
     {
      bool bothGood = true;
      
      // Latitude
      if(!(refPointArray[0].IsNumber()))
       {
        bothGood = false;
        LogOLDFValidity("referencePoint latitude is not numerical in OLDF file %s\n",
                                                                      config.designFileName);
       }
      else
       {
        if(refPointArray[0].GetDouble() < -90.0 || refPointArray[0].GetDouble() > 90.0)
         {
          bothGood = false;
          LogOLDFValidity("referencePoint latitude is out of range in OLDF file %s\n",
                                                                      config.designFileName);
         }
      }

      // Longtitude
      if(!(refPointArray[1].IsNumber()))
       {
        bothGood = false;
        LogOLDFValidity("referencePoint longtitude is not numerical in OLDF file %s\n",
                                                                      config.designFileName);
       }
      else
       {
        if(refPointArray[0].GetDouble() < -180.0 || refPointArray[0].GetDouble() > 180.0)
         {
          bothGood = false;
          LogOLDFValidity("referencePoint longtitude is out of range in OLDF file %s\n",
                                                                      config.designFileName);
         }
      }

      if(bothGood)
        LogOLDFDetails("Reference Point of OLDF file %s is lat %.3f, long %.3f\n",
            config.designFileName, refPointArray[0].GetDouble(), refPointArray[1].GetDouble());
      else
        retVal = false;
     }
    else
     {
      LogOLDFValidity("boundaries:referencePoint array is wrong size %d in OLDF file %s\n",
                                                    refPointArray.Size(), config.designFileName);
     }
   }
  else
   {
    LogOLDFValidity("No boundaries:referencePoint array in OLDF file %s\n", config.designFileName);
    retVal = false;
   }
  
 return retVal;
}


// =======================================================================================
// Function to check the OLDF boundary arcs.

bool Boundary::validateArcs(Value& boundaries)
{
  bool retVal = true;
  const PmodConfig& config = PmodConfig::getConfig();
  float sumDist[2] = {0.0f, 0.0f};
  
  unless(boundaries.HasMember("arcs"))
   {
    LogOLDFDetails("No boundaries:arcs object in OLDF file %s\n", config.designFileName);
    return true;
   }
  
  if(boundaries["arcs"].IsArray())
   {
    Value& arcArray = boundaries["arcs"];
    if(arcArray.Size() >= 3)
     {
      bool allGood = true;
      
      for(int i=0; i<arcArray.Size(); i++)
       {
        if(!(arcArray[i].IsArray()))
         {
          allGood = false;
          LogOLDFValidity("arcs[%d] is not array in OLDF file %s\n", i, config.designFileName);
         }
        else
         {
          if(arcArray[i].Size() == 2)
            for(int j=0; j<2; j++)
             {
              if(arcArray[i][j].IsNumber())
                sumDist[j] += arcArray[i][j].GetFloat();
              else
               {
                allGood = false;
                LogOLDFValidity("arcs[%d][%d] is not numerical in OLDF file %s\n", i, j,
                                                                      config.designFileName);
               }
             }
          else
           {
            LogOLDFValidity("arcs[%d] has wrong size %d in OLDF file %s\n", i,
                                                    arcArray[i].Size(), config.designFileName);
           }
         }
       }
 
      if(allGood)
       {
        if( (fabs(sumDist[0]) < EPSILON) && (fabs(sumDist[1]) < EPSILON) )
          LogOLDFDetails("Correct arcs array of size %d in OLDF file %s\n",
                                                      arcArray.Size(), config.designFileName);
        else
         {
          retVal = false;
          LogOLDFValidity("arcs do not close (sum %f, %f) in OLDF file %s\n", sumDist[0],
                                                        sumDist[1], config.designFileName);
         }
       }
      else
        retVal = false;
     }
    else
     {
      LogOLDFValidity("boundaries:arcs array is too small (size %d) in OLDF file %s\n",
                                                    arcArray.Size(), config.designFileName);
     }
   }
  else
   {
    LogOLDFValidity("No boundaries:arcs array in OLDF file %s\n", config.designFileName);
    retVal = false;
   }
  
 return retVal;
}


// =======================================================================================
// Function to check the structure of the OLDF boundaries object.

bool Boundary::validateBoundaries(Value&  boundaries)
{
  bool    retVal      = true;

  retVal &= validateReferencePoint(boundaries);
  retVal &= validateArcs(boundaries);

  return retVal;
}


// =======================================================================================
// Write out the boundary data to a file in OLDF JSON format.

void Boundary::writeOLDFSection(FILE* file, char* indent)
{
  // Open the object
  fprintf(file, "%s\"boundaries\":\n", indent);
  fprintf(file, "%s {\n", indent);

  // Reference point
  fprintf(file, "%s%s\"referencePoint\": [%f, %f],\n", indent, indent,
                                                    referencePoint[0], referencePoint[1]);

  // Arcs
  fprintf(file, "%s%s\"arcs\":\n", indent, indent);
  fprintf(file, "%s%s [\n", indent, indent);
  int N = arcs.size()/2;
  for(int i=0; i<N; i++)
   {
    fprintf(file, "%s%s%s[%f, %f]", indent, indent, indent, arcs[2*i], arcs[2*i+1]);
    if(i<N-1)
      fprintf(file, ",\n");
    else
      fprintf(file, "\n");
   }
  fprintf(file, "%s%s ]\n", indent, indent);

  // Close out the boundaries object
  fprintf(file, "%s },\n", indent);
}


// =======================================================================================
// Not implemented

bool Boundary::diagnosticHTML(HttpDebug* serv)
{

  return false;
}


// =======================================================================================
