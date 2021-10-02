// Copyright Staniford Systems.  All Rights Reserved.  Jun 2021 -
// This class is responsible for ensuring that all resources (textures, images files,
// solar databases, etc), are present and correct before we try to start up.  It
// first gets the json manifest.  It largely relies on external programs (rsync, curl)
// to do the heavy lifting.

#include "ResourceManager.h"
#include "loadFileToBuf.h"
#include "MainSceneWindow.h"
#include <cstring>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


// =======================================================================================
// Useful constants

#define MATERIALS_DIR (char*)"Materials"
#define MANIFEST_FILE (char*)"manifest.json"
#define PATH_BUF_SIZE 256

using namespace rapidjson;


// =======================================================================================
/// @brief Constructor
///
/// Most of the work in this class occurs in the constructor.  The other methods in the
/// class are helper functions invoked at construction time.
/// @param window A reference to the open window at the time of our invocation (needed 
/// in case we have to do some user-interaction).

ResourceManager::ResourceManager(MainSceneWindow& window)
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
/// @brief Destructor

ResourceManager::~ResourceManager(void)
{

}


// =======================================================================================
/// @brief Recursively check the Materials directory tree matches the manifest.
///
/// Function that walks the directory tree and makes sure that it is as specified in the
/// MANIFEST_FILE.  We ignore any extra directories and files that are not specified in
/// the manifest - assuming they are some other user thing.  But we will fix permissions
/// on the directories we expect.
/// @param directoryTree A rapidson::Value for the directories object in the JSON manifest
/// file.
/// @param path The directory path so far
/// @param pathlen The length of the path so far

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
    snprintf(path + pathlen, PATH_BUF_SIZE-pathlen-1, "%s/", 
                                                  directoryTree["name"].GetString());
    pathlen += strlen(directoryTree["name"].GetString()) + 1;
    int mode;
    if( (mode = modeBitsFromString(directoryTree["mode"].GetString())) < 0)
      err(-1, "Invalid mode %s in %s.\n", directoryTree["mode"].GetString(), path);
    unless(directoryExists(path))
     {
      if(mkdir(path, mode) < 0)
        err(-1, "Couldn't create directory %s.\n", path);
      LogResourceActions("Created directory %s.\n", path);
     }
    else
     {
      unless(checkAndFixPermissions(path, mode))
       {
        LogResourceActions("Corrected permissions on directory %s to %s.\n", 
                                                  path, directoryTree["mode"].GetString());
       }
     }
    if(directoryTree.HasMember("subdirs"))
     {
      unless(directoryTree["subdirs"].IsArray())
        err(-1, "Bad subdir value not array at %s.\n", path);
      checkDirectories(directoryTree["subdirs"], path, pathlen);
     }
    if(directoryTree.HasMember("files"))
      checkFiles(directoryTree["files"], path, pathlen);
   }
}


// =======================================================================================
/// @brief Check the files in a given directory.
/// 
/// Function that is called by ResourceManager::checkDirectories above, and serves to 
/// check for the individual files listed under some given directory.

void ResourceManager::checkFiles(Value& fileList, char* path, unsigned pathlen)
{
  unless(fileList.IsArray())
    err(-1, "Bad, non-array, fileList at %s.\n", path);
  
  int N = fileList.Size();
  for(int i=0; i<N; i++)
    checkOneFile(fileList[i], i, path, pathlen);
}


// =======================================================================================
/// @brief Get one file if needed.
/// 
/// Function that is called by checkFiles above, and serves to handle one specific file
/// and the various sources it might be obtained from.

void ResourceManager::checkOneFile(Value& fileObject, int i, char* path, unsigned pathlen)
{
  unless(fileObject.IsObject())
    err(-1, "Bad file object number %d in %s.\n", i, path);
  unless(fileObject.HasMember("name") && fileObject["name"].IsString())
    err(-1, "No file object name for number %d in %s.\n", i, path);
  unless(fileObject.HasMember("sources") && fileObject["sources"].IsArray()
         && fileObject["sources"].Size() > 0)
    err(-1, "No file object sources for number %d in %s.\n", i, path);
  strncpy(path + pathlen, fileObject["name"].GetString(), PATH_BUF_SIZE-pathlen-1);
  pathlen += strlen(fileObject["name"].GetString());
  bool success = false;
  int j = 0;
  int N = fileObject["sources"].Size();
  while(!success && j < N)
   {
    success = success || httpClient.fetchFile(fileObject["sources"][j].GetString(), path);
    j++;
   }
  unless(success)
    err(-1, "Couldn't find a working source to get %s.\n", path);
}


// =======================================================================================
