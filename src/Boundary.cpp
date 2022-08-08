// Copyright Staniford Systems.  All Rights Reserved.  Feb 2021 -
// Class for storing and working with the boundary of a plot of land, as in the OLDF
// boundary object.

#include "Boundary.h"
#include "PmodConfig.h"
#include "Logging.h"
#include "Global.h"
#include "BoundingBox.h"
#include "HttpDebug.h"
#include "PmodDesign.h"

using namespace rapidjson;


// =======================================================================================
/// @brief Constructor (currently does nothing)

Boundary::Boundary()
{
}


// =======================================================================================
/// @brief Destructor

Boundary::~Boundary(void)
{
}


// =======================================================================================
/// @brief Set us up from an OLDF boundary object.
///
/// The OLDF is assumed validated before being supplied to us (probably by 
/// validateBoundaries below).
/// @param boundaries A reference to the rapidjson::Value with the JSON/OLDF in.

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
/// @brief Extend a bounding box to include all the points in this boundary.
/// @param box A reference to the BoundingBox that we are to extend.

bool Boundary::extendBoundingBox(BoundingBox& box)
{
  bool retVal = false;
  int N = arcs.size()/2;
  for(int i=0; i<2*N; i+=2)
   {
    if(arcs[i] < box.lower[0])
     {
      box.lower[0] = arcs[i];
      retVal = true;
     }
    if(arcs[i] > box.upper[0])
     {
      box.upper[0] = arcs[i];
      retVal = true;
     }
    if(arcs[i+1] < box.lower[1])
     {
      box.lower[1] = arcs[i+1];
      retVal = true;
     }
    if(arcs[i+1] > box.upper[1])
     {
      box.upper[1] = arcs[i+1];
      retVal = true;
     }
   }
  return retVal;
}


// =======================================================================================
/// @brief Find the minimum and maximum extent of our boundary as latitude and longtitude.
/// 
/// This is used for example in asking Permaserv for soil profile information, etc.  Note
/// that the boundary object is confusing, in that reference point is <lat, long> ie kind
/// of <y,x>, whereas the arcs, x comes before y.
/// (possibly climatic information in future).
/// @param loLat A reference to a float to store the lowest latitude.
/// @param hiLat A reference to a float to store the highest latitude.
/// @param loLong A reference to a float to store the lowest longtitude.
/// @param hiLong A reference to a float to store the highest longtitude.

void Boundary::latLongRange(float& loLat, float& hiLat, float& loLong, float& hiLong)
{
  // Handling of the reference point
  float latT = referencePoint[0];
  float longT  = referencePoint[1];
  
  // Other initialization
  loLat = hiLat = latT;
  loLong = hiLong = longT;
  int N = arcs.size()/2;
  float cosLat = cosf(latT);  // latitude circles shrink towards the poles
  
  // Now loop over the arcs, deciding whether or not this expands the rectangle
  //printf("Ref; latT: %f, longT: %f\n", latT, longT);
  for(int i=0; i<2*N; i+=2)
   {
    latT += arcs[i+1]/spaceUnitsPerDegree*cosLat;
    longT += arcs[i]/spaceUnitsPerDegree;
    //printf("i: %d; latT: %f, longT: %f\n", i, latT, longT);
    if(latT < loLat)
      loLat = latT;
    else
      if(latT > hiLat)
        hiLat = latT;
    if(longT < loLong)
      loLong = longT;
    else
      if(longT > hiLong)
        hiLong = longT;
   }
  LogSoilDbDetails("Boundary::latLongRange has Lat: [%.6f, %.6f], Long: [%.6f, %.6f].\n",
                                                            loLat, hiLat, loLong, hiLong);
}


// =======================================================================================
/// @brief Validate the OLDF boundary reference point.
/// @returns True if the OLDF is valid, false otherwise.
/// @param boundaries A reference to the rapidjson::Value with the JSON/OLDF for the 
/// boundaries object in.

bool Boundary::validateReferencePoint(Value& boundaries)
{
  bool retVal = true;
#if defined LOG_OLDF_VALIDITY || defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
  
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
/// @brief Function to check the OLDF boundary arcs.
/// @returns True if the OLDF is valid, false otherwise.
/// @param boundaries A reference to the rapidjson::Value with the JSON/OLDF for the 
/// boundaries object in.

bool Boundary::validateArcs(Value& boundaries)
{
  bool retVal = true;
#if defined LOG_OLDF_VALIDITY || defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
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
/// @brief Function to check the structure of the OLDF boundaries object overall.
/// @returns True if the OLDF is valid, false otherwise.
/// @param boundaries A reference to the rapidjson::Value with the JSON/OLDF for the 
/// boundaries object in.

bool Boundary::validateBoundaries(Value&  boundaries)
{
  bool    retVal      = true;

  retVal &= validateReferencePoint(boundaries);
  retVal &= validateArcs(boundaries);

  return retVal;
}


// =======================================================================================
/// @brief Write out the boundary data to a file in OLDF JSON format.
/// @param file A C style FILE* pointing to the open file to write into.
/// @param indent The string to use for one indentation unit in the JSON/OLDF.

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
/// @brief Put the boundary data in a section of an HTML page.
/// @returns True if the desired HTML was written correctly, false if we ran out of space.
/// @param serv The HTTP Debug server

bool Boundary::diagnosticHTML(HttpDebug* serv)
{
  httPrintf("<h2>Boundary</h2>\n");
  
  // Reference Point
  httPrintf("<b>Reference Point:</b> [%f, %f]<br><br>\n", referencePoint[0],
                                                                    referencePoint[1]);
  // Arcs
  httPrintf("<b>Arcs</b><br>\n");
  httPrintf("<center>\n");
  serv->startTable();
  httPrintf("<tr><th>Index</th><th>Location</th></tr>\n");
  int N = arcs.size()/2;
  for(int i=0; i<N; i++)
  httPrintf("<tr><th>%d</th><th>[%f, %f]</th></tr>\n", i, arcs[2*i], arcs[2*i+1]);
  httPrintf("</table></center><hr>\n");

  return true;
}


// =======================================================================================
