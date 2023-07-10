// Copyright Staniford Systems.  All Rights Reserved.  June 2023 -
// Holder for a taxonomic family (containing genera, which contain species).

#include "Family.h"
#include "Genus.h"
#include "Logging.h"
#include "HttpServThread.h"
#include <string.h>


// =======================================================================================
/// @brief Constructor

Family::Family(char* familyName)
{
  name = strdup(familyName);
}


// =======================================================================================
/// @brief Destructor

Family::~Family(void)
{
  free(name);
}


// =======================================================================================
/// @brief (Possibly) add a new element to this family.
/// @returns True if we added something, false otherwise.
/// @param species The name of the species
/// @param genus The name of the genus

bool Family::add(char* species, char* genus)
{
  if(count(genus))
   {
    //(*this)[genus]->add(species);
    return true;
   }
  else
   {
    Genus* newGenus = new Genus(genus);
    insert({std::string(genus), newGenus});
    LogTaxonDetails("Adding new Genus %s to Family %s.\n", genus, name);
    //newGenus->add(species, genus);
    return true; 
   }
}


/// =======================================================================================
/// @brief Output HTML table of the genera in this family.
/// 
/// @returns True if all was well writing to the buffer.  If false, it indicates the 
/// buffer was not big enough and the output will have been truncated/incomplete.
/// @param serv A pointer to the HttpServThread managing the HTTP response.

bool Family::generaHTMLTable(HttpServThread* serv)
{
  // Start the table
  char tableName[128];
  snprintf(tableName, 128, "Genera_for_family_%s", name);
  unless(serv->startTable(tableName))
    return false;
  httPrintf("<tr><th>Row</th><th>Genus Name</th></tr>\n");
  
  // Iterate over the rows
  int index = 1;
  for (auto i : *this)
   {
    httPrintf("<tr><td>%d</td><td><a href=\"/taxonomy/genus/%s\">%s</a></td></tr>\n", 
                                              index++, i.first.c_str(), i.first.c_str()); 
   }

  // Finish up the table
  httPrintf("</table></center>"); 
  return true;
}


// =======================================================================================
