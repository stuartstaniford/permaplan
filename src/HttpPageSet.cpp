// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -
// Maps a set of paths to a set of instances of HttpStaticPage.  Used for resolving 
// calls to get pages that will be loaded on first request and then cached in memory.  
// We do not just serve anything found in a directory, but only things explicitly 
// coded to be served

#include "HttpPageSet.h"
#include "HttpServThread.h"
#include "HttpStaticPage.h"
#include <assert.h>


// =======================================================================================
/// @brief Constructor
/// @param path A char* pointer to the path where files for this particular set of static
/// pages are to be found.  Should include the trailing slash (eg "scripts/").

HttpPageSet::HttpPageSet(char* path)
{
  unsigned pathSize = strlen(path) + 1;
  assert(pathSize < STAT_URL_BUF_SIZE);
  strncpy(urlBuf, path, pathSize);
  urlBufPtr = urlBuf + pathSize;
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
    strncpy(urlBufPtr, url, STAT_URL_BUF_SIZE - (urlBufPtr-urlBuf));
    HttpStaticPage* page = at(urlBuf);
    if(!page)
      page = new HttpStaticPage(url);
    char* response = page->getResponse();
    serv->setAltResp(response, strlen(response));
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
