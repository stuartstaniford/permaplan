// Copyright Staniford Systems.  All Rights Reserved.  July 2023 -
// Models a Class in the biological sense as part of a Taxonomy.  Inherits from a 
// map of Orders

#include "BioClass.h"
#include "HttpServThread.h"
#include <string.h>


// =======================================================================================
/// @brief Constructor

BioClass::BioClass(char* className)
{
  name = strdup(className);
}


// =======================================================================================
/// @brief Destructor

BioClass::~BioClass(void)
{
  free(name);
}


// =======================================================================================
/// @brief (Possibly) add a new element to this class.
/// @returns True if we added something, false otherwise.
/// @param species The name of the species
/// @param genus The name of the genus
/// @param family The name of the family
/// @param order The name of the order

bool BioClass::add(char* species, char* genus, char* family, char* order)
{
   return true; 
}


/// =======================================================================================
/// @brief Output HTML table of the orders in this class.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool BioClass::orderHTMLTable(HttpServThread* serv)
{
  char tableName[128];
  snprintf(tableName, 128, "Orders_for_class_%s", name);
  unless(serv->startTable(tableName))
    return false;
  httPrintf("<tr><th>Row</th><th>Order Name</th></tr>\n");
  httPrintf("</table></center>"); 
  return true;
}


// =======================================================================================
