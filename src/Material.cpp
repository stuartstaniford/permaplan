// Copyright Staniford Systems.  All Rights Reserved.  October 2020 -
// This class holds the necessary information for a particular kind of material -
// carbon data, texture image for showing the surface of it, etc

#include "Material.h"
#include "Logging.h"
#include <cstdio>
#include <err.h>


// =======================================================================================
// Static variables

MaterialList* MaterialList::theMaterialList = NULL;

// =======================================================================================
// Constructor for individual material

Material::Material(float carbonD, char* matName, TextureAtlas& atlas, char* tFileName):
                      name(matName),
                      carbonDensity(carbonD),
                      texture(tFileName, 0)
{
  if(atlas.pathMap.find(tFileName) != atlas.pathMap.end())
   {
    LogPathMap("Found %s in path map\n", tFileName);
    texCoords = atlas.pathMap[tFileName];
   }
  else
   {
    LogPathMap("Failed to find %s in path map\n", tFileName);
    texCoords = NULL;
   }
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

MaterialList::MaterialList(TextureAtlas& atlas)
{
  // Constructor should only be called once at startup.  Everyone else gets us via
  // getMaterials()
  if(theMaterialList)
    return;
  else
    theMaterialList = this;
  
  (*theMaterialList)[(char*)"StrawBale"] = new Material(0.0, (char*)"StrawBale", atlas,
                                                    (char*)"Materials/Blocks/StrawBale/Bale.png");

  (*theMaterialList)[(char*)"Nexcem"] = new Material(0.0, (char*)"Nexcem", atlas,
                                                    (char*)"Materials/Blocks/Nexcem/nexcem.png");
}


// =======================================================================================
// Destructor for material list

MaterialList::~MaterialList(void)
{
}


// =======================================================================================
