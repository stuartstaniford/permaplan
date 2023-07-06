// Copyright Staniford Systems.  All Rights Reserved.  July 2023 -
// Models a Class in the biological sense as part of a Taxonomy.  Inherits from a 
// map of Orders

#include "BioClass.h"
#include "Order.h"
#include "HttpServThread.h"
#include "Logging.h"
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
  if(count(order))
   {
    //return bioClassesByName[bioClass]->add(species, genus, family, order);
    return true;
   }
  else
   {
    Order* newOrder = new Order(order);
    insert({std::string(order), newOrder});
    LogTaxonDetails("Adding new Order %s to class %s.\n", order, name);
   }

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
  // Start the table
  char tableName[128];
  snprintf(tableName, 128, "Orders_for_class_%s", name);
  unless(serv->startTable(tableName))
    return false;
  httPrintf("<tr><th>Row</th><th>Order Name</th></tr>\n");
  
  // Iterate over the rows
  int index = 1;
  for (auto i : *this)
   {
    httPrintf("<tr><td>%d</td><td><a href=\"/taxonomy/order/%s\">%s</a></td></tr>\n", 
                                              index++, i.first.c_str(), i.first.c_str()); 
   }

  // Finish up the table
  httPrintf("</table></center>"); 
  return true;
}


// =======================================================================================
