// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// Class that provides functionality to load and manage a collection
// of textures into a single texture, and provide appropriate indexing
// and remapping of u,v coords into the subspace within the larger
// texture.  We operate on a directory with sub-directories.  Each
// subdirectory will become a separate atlas, with all the files in
// that subdirectory stored in that particular texture atlas.  The
// end result is a hash table of each "dir/file" as well as the
// atlasses themselves.

#include <err.h>
#include <cstring>
#include "TextureAtlas.h"
#include "Logging.h"


// =======================================================================================
// Constructor

TextureAtlas::TextureAtlas(char* dirName)
{
  // Open the base directory
  DIR* atlasRoot = opendir(dirName);
  if(!atlasRoot)
    err(-1, "Couldn't open directory %s in TextureAtlas::TextureAtlas", dirName);
  
  //Traverse the base directory for subdirectories
  struct dirent* dirEntry;
  DIR* subDir;
  char subDirName[1024];
  while( (dirEntry = readdir(atlasRoot)) )
   {
    if(dirEntry->d_type != DT_DIR)   // ignore regular files and other non-directories
      continue;
    if(strcmp(dirEntry->d_name, ".") == 0  || strcmp(dirEntry->d_name, "..") == 0)
      continue;
    sprintf(subDirName, "%s/%s", dirName, dirEntry->d_name);
    subDir = opendir(subDirName);
    if(!subDir)
      err(-1, "Couldn't open directory %s in TextureAtlas::TextureAtlas", subDirName);
    LogTextureAtlas("Creating texture atlas for %s.\n", subDirName);
    processOneAtlas(subDir, subDirName);
    closedir(subDir);
   }
  closedir(atlasRoot);
}


// =======================================================================================
// Deal with one particular directory, and the atlas created from the textures in it.

void TextureAtlas::processOneAtlas(DIR* dir, char* path)
{
  struct dirent* dirEntry;
  while( (dirEntry = readdir(dir)) )
   {
    if(dirEntry->d_type != DT_REG)   // ignore anything but regular files
      continue;
    if(dirEntry->d_name[0] == '.') // ignore hidden files starting with "."
      continue;
    
    // HERE
    //sprintf(subDirName, "%s/%s", dirName, dirEntry->d_name);
    //subDir = opendir(subDirName);
   }
}


// =======================================================================================
// Destructor

TextureAtlas::~TextureAtlas(void)
{
}


// =======================================================================================
