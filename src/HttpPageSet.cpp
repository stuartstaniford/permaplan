// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -
// Maps a set of paths to a set of instances of HttpStaticPage.  Used for resolving 
// calls to get pages that will be loaded on first request and then cached in memory.  
// We do not just serve anything found in a directory, but only things explicitly 
// coded to be served

#include "HttpPageSet.h"
#include "HttpServThread.h"
#include "HttpStaticPage.h"


// =======================================================================================
/// @brief Constructor

HttpPageSet::HttpPageSet(void)
{
}


// =======================================================================================
/// @brief Destructor

HttpPageSet::~HttpPageSet(void)
{
}


// =======================================================================================
/// @brief Provide a page, if we have it
/// 
/// @returns true if all went well, false if we ran out of space or other problem.

bool HttpPageSet::processPageRequest(HttpServThread* serv, char* url)
{
  lock();
  if(count(url))
   {
    HttpStaticPage* page = at(url);
    if(!page)
      page = new HttpStaticPage(url);
    char* response = page->getResponse();
    unlock();
    return true;
   }
  else
   {
    unlock();
    return serv->errorPage("Resource Not Found.");
   }
}


// =======================================================================================
