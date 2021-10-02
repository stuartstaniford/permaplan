// Copyright Staniford Systems.  All Rights Reserved.  October 2020 -
// This class holds the necessary information for a particular kind of material -
// carbon data, texture image for showing the surface of it, etc

#include "Material.h"
#include "Logging.h"
#include "TextureAtlas.h"
#include "HttpDebug.h"
#include <cstdio>
#include <err.h>


// =======================================================================================
// Static variables

MaterialList* MaterialList::theMaterialList = NULL;


// =======================================================================================
/// @brief Constructor for individual material
/// @param carbonD The float carbon density of this material
/// @param matName A C-string for the name of the material
/// @param atlas A reference to a TextureAtlas containing the texture for the material
/// @param tFileName A C-string filename for the texture for this material

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
/// @brief Destructor for individual material

Material::~Material(void)
{
}


// =======================================================================================
/// @brief Constructor for a list of materials
/// @param atlas A reference to a TextureAtlas
/// @todo Currently this is hard-coded in a local location.  Ultimate plan is this 
/// stuff should be downloaded from a shared repository at startup and cached (via the
/// ResourceManager class).

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
/// @brief Destructor for material list

MaterialList::~MaterialList(void)
{
}


// =======================================================================================
