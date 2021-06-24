// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is responsible for ensuring that all resources (textures, images files,
// solar databases, etc), are present and correct before we try to start up.  It
// first gets the json manifest.  It largely relies on external programs (rsync, curl)
// to do the heavy lifting.

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Window3D.h"
#include "HttpClient.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

// =======================================================================================
// Class variable initialization

class ResourceManager
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ResourceManager(Window3D& window);
  ~ResourceManager(void);
  void checkDirectories(rapidjson::Value& directoryTree, char* path, unsigned pathlen);
  void checkFiles(rapidjson::Value& fileList, char* path, unsigned pathlen);

private:
  
  // Instance variables - private
  HttpClient    httpClient;
  
  // Member functions - private
  ResourceManager(const ResourceManager&);                 // Prevent copy-construction
  ResourceManager& operator=(const ResourceManager&);      // Prevent assignment

};

#endif




