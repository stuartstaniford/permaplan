// Copyright Staniford Systems.  All Rights Reserved.  October 2020 -
// This class holds the necessary information for a particular kind of material -
// carbon data, texture image for showing the surface of it, etc

#include <cstdio>
#include <err.h>
#include "Material.h"
#include "Logging.h"

MaterialList* MaterialList::theMaterialList = NULL;

// =======================================================================================
// Constructor for individual material

Material::Material(float carbonD, char* matName, char* tFileName):
                      name(matName),
                      carbonDensity(carbonD),
                      texture(tFileName, 0)
{
}


// =======================================================================================
// Destructor for individual material

Material::~Material(void)
{
}


// =======================================================================================
// Constructor for list of materials
// XX Currently this is hard-coded in a local location.  Ultimate plan is this stuff
// should be downloaded from a shared repository at startup and cached.

MaterialList::MaterialList(TextureAtlas& blocksAtlas)
{
  // Constructor should only be called once at startup.  Everyone else gets us via
  // getMaterials()
  if(theMaterialList)
    return;
  else
    theMaterialList = this;
  
  // StrawBale
  char* strawBalePath = (char*)"Materials/Blocks/StrawBale/Bale.png";
  if(blocksAtlas.pathMap.find(strawBalePath) != blocksAtlas.pathMap.end())
   {
    LogPathMap("Found %s in path map\n", strawBalePath);
   }
  else
    LogPathMap("Failed to find %s in path map\n", strawBalePath);

  Material* strawBale = new Material(0.0, (char*)"StrawBale", strawBalePath);
  (*theMaterialList)[(char*)"StrawBale"] = strawBale;

  //Nexcem
  char* nexcemPath = (char*)"Materials/Blocks/Nexcem/nexcem.png";
  if(blocksAtlas.pathMap.find(nexcemPath) != blocksAtlas.pathMap.end())
   {
    LogPathMap("Found %s in path map\n", nexcemPath);
   }
  else
    LogPathMap("Failed to find %s in path map\n", nexcemPath);
  Material* nexcem = new Material(0.0, (char*)"Nexcem", nexcemPath);
  (*theMaterialList)[(char*)"Nexcem"] = nexcem;
}


// =======================================================================================
// Destructor for material list

MaterialList::~MaterialList(void)
{
}


// =======================================================================================
