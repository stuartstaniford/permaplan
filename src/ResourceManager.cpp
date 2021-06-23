// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is responsible for ensuring that all resources (textures, images files,
// solar databases, etc), are present and correct before we try to start up.  It
// first gets the json manifest.  It largely relies on external programs (rsync, curl)
// to do the heavy lifting.

#include "ResourceManager.h"
#include "loadFileToBuf.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MATERIALS_DIR (char*)"Materials"
#define MANIFEST_FILE (char*)"manifest.json"

using namespace rapidjson;

// =======================================================================================
// Constructor

#define PATH_BUF_SIZE 256

ResourceManager::ResourceManager(Window3D& window)
{
  char buf[PATH_BUF_SIZE];
  // First check if the Materials directory exists, create it if necessary and approved
  if(!directoryExists(MATERIALS_DIR))
   {
    // It's possible we are just being run in the wrong place - double check.
    getcwd(buf, PATH_BUF_SIZE);
    LogResourceErrors("Directory %s not found in %s.", MATERIALS_DIR, buf);
    char question[192];
    snprintf(question, 192, 
             "Directory %s does not exist in %s.  Create and fetch resources?",
             MATERIALS_DIR, buf);
    char* questionResponses[2] = {(char*)"Cancel", (char*)"OK"};
    int response = window.initPanel(question, questionResponses, 2);
    fprintf(stderr, "Response was %d\n", response);
    if(response <= 0)
      err(-1, "No %s directory in %s.\n", MATERIALS_DIR, buf);
    mkdir(MATERIALS_DIR, S_IRWXU|S_IRWXG);
   }
  DIR* dir;
  dir = opendir(MATERIALS_DIR);
  
  // Check if the manifest.json file exists, fetch it if necessary.
  snprintf(buf, PATH_BUF_SIZE, "%s/%s", MATERIALS_DIR, MANIFEST_FILE);
  unless(regularFileExists(buf))
   {
    ; // XX need website where this would live
   }
  
  // Now execute finding all the things.
  unsigned manifestSize;
  char* manifestBuf = loadFileToBuf(buf, &manifestSize);
  
  Document  doc;
  ParseResult ok = doc.ParseInsitu<kParseCommentsFlag>(manifestBuf);
  if (!ok)
   {
    fprintf(stderr, "JSON parse error on %s file: %s (%u)\n",
            MANIFEST_FILE, GetParseError_En(ok.Code()), (unsigned)(ok.Offset()));
    exit(1);
   }
  if(!doc.IsObject())
    err(-1, "Base of JSON file %s is not JSON object.\n", MANIFEST_FILE);
  unless(doc.HasMember("directories") && doc["directories"].IsArray())
    err(-1, "No directories object in %s.\n", MANIFEST_FILE);
  
  int sz = strlen(MATERIALS_DIR);
  buf[sz+1] = '\0';
  checkDirectories(doc["directories"], buf, sz+1);
}


// =======================================================================================
// Destructor

ResourceManager::~ResourceManager(void)
{

}


// =======================================================================================
// Function that walks the directory tree and makes sure that it is as specified in the
// MANIFEST_FILE.  We ignore any extra directories and files that are not specified in
// the manifest - assuming they are some other user thing.  But we will fix permissions
// on the directories we expect.

void ResourceManager::checkDirectories(Value& directoryTree, char* path, unsigned pathlen)
{
  if(directoryTree.IsArray())
   {
    int N = directoryTree.Size();
    for(int i=0; i<N; i++)
     {
      unless(directoryTree[i].IsObject())
        err(-1, "Non object in directory tree %s.", path);
      checkDirectories(directoryTree[i], path, pathlen);
     }
   }
  else if(directoryTree.IsObject())
   {
    unless(directoryTree.HasMember("name"))
      err(-1, "Object has no name in directory %s.\n", path);
    unless(directoryTree.HasMember("mode"))
      err(-1, "Object %s has no mode in directory %s.\n", 
                                            directoryTree["name"].GetString(), path);
    snprintf(path + pathlen, PATH_BUF_SIZE-pathlen-1, "%s/", directoryTree["name"].GetString());
    unless(directoryExists(path))
     {
      if(mkdir(path, S_IRWXU|S_IRWXG) < 0)
        err(-1, "Couldn't creat directory %s.\n", path);
      LogResourceActions("ResourceManager created directory %s.\n", path);
      
     }
   }
}


// =======================================================================================
