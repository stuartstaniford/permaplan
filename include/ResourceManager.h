// Copyright Staniford Systems.  All Rights Reserved.  June 2021 -

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Window3D.h"
#include "HttpClient.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"


// =======================================================================================
/// @brief Fetch/manage external resources.
///
/// This class is responsible for ensuring that all resources (textures, images files,
/// solar databases, etc), are present and correct before we try to start up.  It
/// first gets the json manifest.  Uses libcurl for data fetching.

class ResourceManager
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ResourceManager(Window3D& window);
  ~ResourceManager(void);

private:
  
  // Instance variables - private
  HttpClient    httpClient;
  
  // Member functions - private
  void checkDirectories(rapidjson::Value& directoryTree, char* path, unsigned pathlen);
  void checkFiles(rapidjson::Value& fileList, char* path, unsigned pathlen);
  void checkOneFile(rapidjson::Value& fileObject, int i, char* path, unsigned pathlen);

  /// @brief Prevent copy-construction
  ResourceManager(const ResourceManager&);  
  /// @brief Prevent assignment
  ResourceManager& operator=(const ResourceManager&);      

};

#endif




