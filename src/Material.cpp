// Copyright Staniford Systems.  All Rights Reserved.  October 2020 -
// This class holds the necessary information for a particular kind of material -
// carbon data, texture image for showing the surface of it, etc

#include <cstdio>
#include <err.h>
#include "Material.h"


// =======================================================================================
// Constructor

Material::Material(float carbonD, char* tFileName):
                      carbonDensity(carbonD),
                      texture(tFileName)
{
}




// =======================================================================================
// Destructor

Material::~Material(void)
{
}


// =======================================================================================
