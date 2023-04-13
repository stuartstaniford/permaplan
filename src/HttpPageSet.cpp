// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -
// Maps a set of paths to a set of instances of HttpStaticPage.  Used for resolving 
// calls to get pages that will be loaded on first request and then cached in memory.  
// We do not just serve anything found in a directory, but only things explicitly 
// coded to be served

#include "HttpPageSet.h"
#include "HttpServThread.h"
#include "HttpStaticPage.h"
#include "Logging.h"
#include <assert.h>


// =======================================================================================
/// @brief Constructor
/// @param path A char* pointer to the path where files for this particular set of static
/// pages are to be found.  Should include the trailing slash (eg "scripts/").
/// @param mType A char* pointer to the mimeType of the files to be served from this 
/// particular set of pages (a given HttpPageSet cannot mix different types of files).

HttpPageSet::HttpPageSet(char* path, MimeType mType): mimeType(mType)
{
  unsigned pathSize = strlen(path);
  assert(pathSize < STAT_URL_BUF_SIZE - 20);
  strcpy(urlBuf, path);
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
/// @param serv pointer to the HTTP serv thread object we need to send our page too.
/// @param url A char* pointer to the url being requested (relative to the path of this
/// particular HttpPageSet).  Note only urls we have been preconfigured to serve will be
/// served, so we don't need to explicitly check for "../" and the like.
/// @todo The result of the strlen could be cached to make this a tiny bit more efficient.

bool HttpPageSet::processPageRequest(HttpServThread* serv, char* url)
{
  lock();
  if(count(url))
   {
    HttpStaticPage* page = at(url);
    
    // Load the page if necessary
    if(!page)
     {
      strncpy(urlBufPtr, url, STAT_URL_BUF_SIZE - (urlBufPtr-urlBuf));
      page = new HttpStaticPage(urlBuf);
      (*this)[url] = page;
     }
    
    // Determine the mime type if necessary
    MimeType mType;
    unless(mimeType == NoMimeType)
      mType = mimeType;
    else
     {
      // Try to get it from extension
      const char* ptr = rindex(url, '.');
      if(ptr && *(++ptr) != '\0')
       {
        ExtensionMimeTypeMap& map = ExtensionMimeTypeMap::getMap();
        unless(map.count(ptr))
         {
          mType = ApplicationOctetStream; // may not be a great solution
          LogResponseErrors("Delivering response with default mime type "
                                              "for unknown extention %s.\n", ptr);
         }
        else
          mType = map[ptr];
       }
      else
       {
        mType = ApplicationOctetStream; // may not be a great solution
        LogResponseErrors("Delivering response with default mime type "
                                                          "to request for %s.\n", url);
       }
     }
    
    // Set up the response for delivery to the client
    char* response = page->getResponse();
    serv->setAltResp(response, strlen(response), mType); 
    serv->setCacheDuration(CACHE_DURATION);
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
