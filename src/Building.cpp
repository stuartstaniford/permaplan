// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class exists to allow us to track the relationship of a series of gables, sheds,
// etc that are managed as a single things (unless disaggregated to manage the
// relationship of the individual pieces.

#include "Building.h"
#include "PmodConfig.h"
#include "Gable.h"
#include "Shed.h"
#include "Box.h"

bool Building::buildingsPresent = false;


// =======================================================================================
/// @brief Constructor

Building::Building(VisualObject* firstComponent):
                          ObjectGroup(firstComponent)
{
}


// =======================================================================================
/// @brief Destructor

Building::~Building(void)
{
}


// =======================================================================================
/// @brief Check OLDF building object.
/// 
/// Static function to co-ordinate the overall checking of the structure of the OLDF 
/// building object.  Checks the master structure, then calls relevant functions to 
/// check the structure of Gables, Sheds, etc present.
/// @returns true if the building OLDF is correct, false otherwise.

using namespace rapidjson;

bool Building::validateBuildings(Value& buildings)
{
  bool    retVal  = true;
#if defined LOG_OLDF_VALIDITY //|| defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
  
  unless(buildings.IsArray())
   {
    LogOLDFValidity("Buildings is not an array in OLDF file %s\n", config.designFileName);
    return false;
   }
  
  buildingsPresent = true;
  int N = buildings.Size();
  
  for(int i = 0; i < N; i++)
   {
    Value& buildingJson = buildings[i];
    unless(buildingJson.IsObject())
     {
      LogOLDFValidity("Buildings[%d] is not an object in OLDF file %s\n", 
                                                          i, config.designFileName);
      return false;
     }
    unless(buildingJson.HasMember("type"))
     {
      LogOLDFValidity("Buildings[%d] has no type in OLDF file %s\n", 
                                                         i, config.designFileName);
      return false;
     }
    unless(buildingJson["type"].IsString())
     {
      LogOLDFValidity("Buildings[%d] has non-string type in OLDF file %s\n", 
                                                         i, config.designFileName);
      return false;
     }
    const char* typeString = buildingJson["type"].GetString();
    if(strcmp(typeString, "gables") == 0)
      retVal &= Gable::validateOLDF(buildingJson);
   }
  
  /*
  if(buildings.HasMember("sheds"))
   {
    shedsPresent = true;
    unless(buildings["sheds"].IsArray())
     {
      LogOLDFValidity("sheds is not an array in OLDF file %s\n", config.designFileName);
      return false;     
     }
    int N = buildings["sheds"].Size();
    for(int i=0; i<N; i++)
      retVal &= Shed::validateOLDF(buildings["sheds"][i]);
   }
  */
  
/*  if(buildings.HasMember("blocks"))
   {
    blocksPresent = true;
    unless(buildings["blocks"].IsArray())
     {
      LogOLDFValidity("blocks is not an array in OLDF file %s\n", config.designFileName);
      return false;     
     }
    int N = buildings["blocks"].Size();
    for(int i=0; i<N; i++)
      retVal &= Box::validateOLDF(buildings["blocks"][i]);
   }
*/
  return retVal;
}


// =======================================================================================
// Write out the buildings section of the OLDF file.

void Building::writeBuildings(FILE* writeFile, char* indent)
{
  unless(buildingsPresent)
    return;
  
  // Open the array
  fprintf(writeFile, "%s\"buildings\":\n", indent);
  fprintf(writeFile, "%s [\n", indent);
  
  // Search for and output buildings here.
  
  fprintf(writeFile, "%s ],\n", indent);
}


// =======================================================================================
