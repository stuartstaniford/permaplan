// Copyright Staniford Systems.  All Rights Reserved.  July 2023 -
// Models a Class in the biological sense as part of a Taxonomy.  Inherits from a 
// map of Orders

#include "BioClass.h"
#include "HttpServThread.h"


// =======================================================================================
/// @brief Constructor

BioClass::BioClass(void)
{
}


// =======================================================================================
/// @brief Destructor

BioClass::~BioClass(void)
{
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
  unless(serv->startTable((char*)"Orders"))
    return false;
  httPrintf("<tr><th>Row</th><th>Order Name</th></tr>\n");
  httPrintf("</table></center>"); 
  return true;
}


// =======================================================================================
