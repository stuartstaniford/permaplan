// Copyright Staniford Systems.  All Rights Reserved.  September 2022 -
// Interface to the Global Historical Climatology Network - Daily data.  This provides 
// global harmonized data for recent daily climate from all over the world.  The purpose 
// of this class is to provide the data in a usable form to the rest of permaserv.  For 
// more on the dataset:
// https://www.ncei.noaa.gov/metadata/geoportal/rest/metadata/item/gov.noaa.ncdc:C00861/html

#include "GHCNDatabase.h"


// =======================================================================================
/// @brief Constructor

GHCNDatabase::GHCNDatabase(void)
{
  readStations();
}


// =======================================================================================
/// @brief Destructor

GHCNDatabase::~GHCNDatabase(void)
{
}


// =======================================================================================
/// @brief Read all the stations into our data structures.
///
/// For more detail, search for 'IV. FORMAT OF "ghcnd-stations.txt"' in
///  https://www.ncei.noaa.gov/pub/data/ghcn/daily/readme.txt

// ------------------------------
// Variable   Columns   Type
// ------------------------------
// ID            1-11   Character
// LATITUDE     13-20   Real
// LONGITUDE    22-30   Real
// ELEVATION    32-37   Real
// STATE        39-40   Character
// NAME         42-71   Character
// GSN FLAG     73-75   Character
// HCN/CRN FLAG 77-79   Character
// WMO ID       81-85   Character
// ------------------------------

void GHCNDatabase::readStations(void)
{
  
}

// =======================================================================================
