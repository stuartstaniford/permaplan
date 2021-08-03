// Copyright Staniford Systems.  All Rights Reserved.  July 2021 -
// This class exists to allow us to track the relationship of a series of gables, sheds,
// etc that are managed as a single things (unless disaggregated to manage the
// relationship of the individual pieces.

#include "Building.h"
#include "PmodConfig.h"
#include "Gable.h"
#include "Shed.h"
#include "Box.h"

bool Building::blocksPresent = false;
bool Building::gablesPresent = false;
bool Building::shedsPresent  = false;

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
  
  unless(buildings.IsObject())
   {
    LogOLDFValidity("Buildings is not an object in OLDF file %s\n", config.designFileName);
    return false;
   }
  
  if(buildings.HasMember("blocks"))
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

  if(buildings.HasMember("gables"))
   {
    gablesPresent = true;
    unless(buildings["gables"].IsArray())
     {
      LogOLDFValidity("gables is not an array in OLDF file %s\n", config.designFileName);
      return false;     
     }
    int N = buildings["gables"].Size();
    for(int i=0; i<N; i++)
      retVal &= Gable::validateOLDF(buildings["gables"][i]);
   }

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
  
  return retVal;
}


// =======================================================================================
// Write out the buildings section of the OLDF file.

void Building::writeBuildings(FILE* writeFile, char* indent)
{
  unless(blocksPresent || gablesPresent || shedsPresent)
    return;
  
  // Open the object
  fprintf(writeFile, "%s\"buildings\":\n", indent);
  fprintf(writeFile, "%s {\n", indent);

  // blocks
  if(blocksPresent)
   {     
    fprintf(writeFile, "%s%s\"blocks\":\n", indent, indent);
    fprintf(writeFile, "%s%s[\n", indent, indent);
    //XX need to search quadtree for all blocks and put them here
    fprintf(writeFile, "%s%s],\n", indent, indent);
   }

  // blocks
  if(gablesPresent)
   {     
    fprintf(writeFile, "%s%s\"gables\":\n", indent, indent);
    fprintf(writeFile, "%s%s[\n", indent, indent);
    //XX need to search quadtree for all gables and put them here
    fprintf(writeFile, "%s%s],\n", indent, indent);
   }

  // sheds
  if(shedsPresent)
   {     
    fprintf(writeFile, "%s%s\"sheds\":\n", indent, indent);
    fprintf(writeFile, "%s%s[\n", indent, indent);
    //XX need to search quadtree for all sheds and put them here
    fprintf(writeFile, "%s%s]\n", indent, indent); // no trailing comma on this one
   }

  fprintf(writeFile, "%s },\n", indent);
}


// =======================================================================================
