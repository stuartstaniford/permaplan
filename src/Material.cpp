// Copyright Staniford Systems.  All Rights Reserved.  October 2020 -
// This class holds the necessary information for a particular kind of material -
// carbon data, texture image for showing the surface of it, etc

#include <cstdio>
#include <err.h>
#include "Material.h"

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

MaterialList::MaterialList(void)
{
  // Constructor should only be called once at startup.  Everyone else gets us via
  // getMaterials()
  if(theMaterialList)
    return;
  else
    theMaterialList = this;
  
  Material* strawBale = new Material(0.0, (char*)"StrawBale",
                                          (char*)"Materials/Blocks/StrawBale/Bale.png");
  (*theMaterialList)[(char*)"StrawBale"] = strawBale;
  
  Material* nexcem = new Material(0.0, (char*)"Nexcem",
                                       (char*)"Materials/Blocks/Nexcem/nexcem.png");
  (*theMaterialList)[(char*)"Nexcem"] = nexcem;
}


// =======================================================================================
// Destructor for material list

MaterialList::~MaterialList(void)
{
}


// =======================================================================================
