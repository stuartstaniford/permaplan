// Copyright Staniford Systems.  All Rights Reserved.  July 2023 -
// Models a biological order.  Inherits from an unordered map of names to Families

#include "Order.h"
#include "Family.h"
#include "HttpServThread.h"
#include "Logging.h"
#include <string.h>


// =======================================================================================
/// @brief Constructor

Order::Order(char* orderName)
{
  name = strdup(orderName);
}


// =======================================================================================
/// @brief Destructor

Order::~Order(void)
{
  free(name);
}


// =======================================================================================
/// @brief (Possibly) add a new element to this order.
/// @returns True if we added something, false otherwise.
/// @param species The name of the species
/// @param genus The name of the genus
/// @param family The name of the family

bool Order::add(char* species, char* genus, char* family)
{
  if(count(family))
   {
    (*this)[family]->add(species, genus);
    return true;
   }
  else
   {
    Family* newFamily = new Family(family);
    insert({std::string(family), newFamily});
    newFamily->add(species, genus);
    LogTaxonDetails("Adding new Family %s to order %s.\n", family, name);
   }

   return true; 
}


/// =======================================================================================
/// @brief Output HTML page of families/genuses known to us.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool Order::provideOrderPage(HttpServThread* serv)
{  
  // Start the HTML page and the table header
  char title[256];
  snprintf(title, 256, "Families in Order %s", name);
  unless(serv->startResponsePage(title))
    return false;

  // Loop over the families
  int famIndex = 1;
  for (auto i : *this)
   {
    char title[128];
    snprintf(title, 128, "Family %d: %s", famIndex++, i.first.c_str());
    unless(serv->newSection(title))
      return false;
    unless(i.second->generaHTMLTable(serv))
      return false;
   }
  
  // Finish up the page
  unless(serv->endResponsePage())
    return false;

  return true;
}


// =======================================================================================
