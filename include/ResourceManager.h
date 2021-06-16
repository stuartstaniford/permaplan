// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is responsible for ensuring that all resources (textures, images files,
// solar databases, etc), are present and correct before we try to start up.  It
// first gets the json manifest.  It largely relies on external programs (rsync, curl)
// to do the heavy lifting.

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

// =======================================================================================
// Class variable initialization

class ResourceManager
{
public:
  
  // Instance variables - public
  
  // Member functions - public
  ResourceManager(void);
  ~ResourceManager(void);
  
private:
  
  // Instance variables - private
  
  // Member functions - private
  ResourceManager(const ResourceManager&);                 // Prevent copy-construction
  ResourceManager& operator=(const ResourceManager&);      // Prevent assignment

};

#endif




