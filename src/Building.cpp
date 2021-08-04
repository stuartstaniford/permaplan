// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class exists to allow us to track the relationship of a series of gables, sheds,
// etc that are managed as a single things (unless disaggregated to manage the
// relationship of the individual pieces.

#include "Building.h"
#include "PmodConfig.h"
#include "Gable.h"
#include "Shed.h"
#include "Box.h"
#include "JSONStructureChecker.h"

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
/// @brief Check a single OLDF building object inside the main array.
/// 
/// Static function to co-ordinate the overall checking of the structure of an OLDF 
/// building object.  Checks the master structure, then calls relevant functions to 
/// check the structure of Gables, Sheds, etc present.
/// @returns true if the building OLDF is correct, false otherwise.
/// @param buildingJson rapidjson::Value reference to the JSON object for this building.
/// @param jCheck JSONStructureChecker helper object for validating/logging.
/// @param i Index of this building in the buildings array.

using namespace rapidjson;

bool Building::validateOneBuilding(Value& buildingJson, int i, JSONStructureChecker* jCheck)
{
  bool    retVal  = true;
#if defined LOG_OLDF_VALIDITY //|| defined LOG_OLDF_DETAILS
  const PmodConfig& config = PmodConfig::getConfig();
#endif
  unless(buildingJson.IsObject())
   {
    LogOLDFValidity("Buildings[%d] is not an object in OLDF file %s\n", 
                                                        i, config.designFileName);
    return false;
   }
  char objName[16];
  snprintf(objName, 16, "Buildings[%d]", i);
  
  // Name
  retVal &= jCheck->validateStringMemberExists(buildingJson, objName, (char*)"name");
  
  // Note
  retVal &= jCheck->validateOptionalStringMember(buildingJson, objName, (char*)"note");
  
  // Position
  unless(buildingJson.HasMember("position"))
   {
    LogOLDFValidity("Buildings[%d] has no position in OLDF file %s\n", 
                                                        i, config.designFileName);
    retVal = false;
   }
  else
    retVal &= jCheck->validateNumberArray(buildingJson["position"], 3, objName);
  
  // AngleFromNorth
  retVal &= jCheck->validateFloatMemberExists(buildingJson, objName, (char*)"angleFromNorth");

  // Assemblies
  unless(buildingJson.HasMember("assemblies"))
   {
    LogOLDFValidity("Buildings[%d] has no assemblies array in OLDF file %s\n", 
                                                        i, config.designFileName);
    retVal =  false;
   }
  unless(buildingJson["assemblies"].IsArray() && buildingJson["assemblies"].Size() > 0)
   {
    LogOLDFValidity("Buildings[%d] has assemblies bad array in OLDF file %s\n", 
                                                      i, config.designFileName);
    retVal =  false;
   }
  
  int N = buildingJson["assemblies"].Size();
  for(int j=0; j< N; j++)
   {
    // Process the j'th sub-assembly in the i'th building.
    Value& assembly = buildingJson["assemblies"][j];
    unless(assembly.HasMember("type"))
     {
      LogOLDFValidity("Buildings[%d] assembly %d has no type in OLDF file %s\n", 
                                                       i, j, config.designFileName);
      return false;
     }
    unless(assembly["type"].IsString())
     {
      LogOLDFValidity("Buildings[%d] assembly %d has non-string type in OLDF file %s\n", 
                                                       i, j, config.designFileName);
      return false;
     }
    const char* typeString = assembly["type"].GetString();
    if(strcmp(typeString, "gable") == 0)
      retVal &= Gable::validateOLDF(buildingJson);
    else if(strcmp(typeString, "shed") == 0)
      retVal &= Gable::validateOLDF(buildingJson);
    else if(strcmp(typeString, "block") == 0)
      retVal &= Gable::validateOLDF(buildingJson);
   }
  
  return retVal;
}


// =======================================================================================
/// @brief Check OLDF building object.
/// 
/// Static function to co-ordinate the overall checking of the structure of the OLDF 
/// building object.
/// @returns true if the building OLDF is correct, false otherwise.
/// @param buildings rapidjson::Value reference to the JSON array.
/// @param jCheck JSONStructureChecker helper object for validating/logging.

bool Building::validateBuildings(Value& buildings, JSONStructureChecker* jCheck)
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
    retVal &= validateOneBuilding(buildings[i], i, jCheck);
  
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
