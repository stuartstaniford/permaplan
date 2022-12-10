// Copyright Staniford Systems.  All Rights Reserved.  December 2022 -
// This class is for the storage of static objects that will be served via the HTTP 
// servers.  On construction, we will load our object from disk, but keep it around 
// so we can be served as needed 

#include "HttpStaticPage.h"
#include "loadFileToBuf.h"


// =======================================================================================
/// @brief Constructor

HttpStaticPage::HttpStaticPage(char* objectPath):originalPath(objectPath)
{
  respBuf = loadFileToBuf(objectPath, &respBufSize);
}


// =======================================================================================
/// @brief Destructor

HttpStaticPage::~HttpStaticPage(void)
{
}


// =======================================================================================
