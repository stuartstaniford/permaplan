// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is responsible for ensuring that all resources (textures, images files,
// solar databases, etc), are present and correct before we try to start up.  It
// first gets the json manifest.  It largely relies on external programs (rsync, curl)
// to do the heavy lifting.

#include "ResourceManager.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MATERIALS_DIR (char*)"Materials"
// =======================================================================================
// Constructor

ResourceManager::ResourceManager(Window3D& window)
{
  // First check if the Materials directory exists, create it if necessary
  DIR* dir;
  dir = opendir(MATERIALS_DIR);
  if(!dir)
   {
    // It's possible we are just being run in the wrong place - double check.
    char cwd[96];
    getcwd(cwd, 96);
    char question[128];
    snprintf(question, 128, 
             "Directory %s does not exist in %s.  Create and fetch resources?",
             cwd, MATERIALS_DIR);
    char* questionResponses[2] = {(char*)"Cancel", (char*)"OK"};
    int response = window.initPanel(question, questionResponses, 2);
    if(!response)
      err(-1, "No %s directory in %s.\n", MATERIALS_DIR, cwd);
    mkdir(MATERIALS_DIR, S_IRWXU|S_IRWXG);
    
   }
  
  // Check if the manifest.json file exists, fetch it if necessary.
  
  // Now execute finding all the things.
  
}


// =======================================================================================
// Destructor

ResourceManager::~ResourceManager(void)
{
}


// =======================================================================================
