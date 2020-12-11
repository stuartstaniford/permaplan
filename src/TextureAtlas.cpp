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
#include "GlobalMacros.h"

#define TexPathLimit 2048

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
  char subDirName[TexPathLimit];
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
// Helper function to see if the file name of an extension might reasonably be an image.

bool extensionCheck(char* fileName)
{
  char* extension = rindex(fileName, '.');
  unsigned len    = strlen(fileName);
  
  if(!extension)      // treat things with no extension as though they might be an image
   {
    LogAtlasAnomalies("Texture Atlas file with no extension %s.\n", fileName);
    return true;
   }
  
  extension++;
  if(extension - fileName >= len)
    err(-1, "Filename %s ends in .\n", fileName);
  
  // Definitely image types
  if(strcmp(extension, "jpg")==0)
    return true;
  if(strcmp(extension, "png")==0)
    return true;
  if(strcmp(extension, "gif")==0)
    return true;
  if(strcmp(extension, "bmp")==0)
    return true;

  // Definitely not image types
  if(strcmp(extension, "json")==0)
    return false;
  if(strcmp(extension, "txt")==0)
    return false;
  if(strcmp(extension, "html")==0)
    return false;
  if(strcmp(extension, "rtf")==0)
    return false;

  LogAtlasAnomalies("Texture Atlas file with unknown extension %s.\n", fileName);
  return true; // default to trying to see if it will work
}


// =======================================================================================
// Deal with one particular directory, and the atlas created from the textures in it.

void TextureAtlas::processOneAtlas(DIR* dir, char* path)
{
  struct dirent* dirEntry;
  Texture* texture;

  while( (dirEntry = readdir(dir)) )
   {
    unless(dirEntry->d_type == DT_REG || dirEntry->d_type == DT_DIR)
      continue;                       // ignore anything but regular files and subdirs
    if(dirEntry->d_name[0] == '.')
      continue;                       // ignore hidden files starting with "."
    if(dirEntry->d_type == DT_REG && !extensionCheck(dirEntry->d_name))
      continue;                       // ignore meta-data files etc.
    int len = strlen(path);
    if(TexPathLimit - len < 4)
      err(-1, "Out of path space for adding %s to %s in processOneAtlas.\n",
                                                    dirEntry->d_name, path);
    path[len] = '/';
    path[len+1] = '\0';
    strncat(path, dirEntry->d_name, TexPathLimit-len-3);
    
    if(dirEntry->d_type == DT_REG)
     {
      texture = new Texture(path);
      LogTextureAtlas("Found %s for texture atlas (width %d, height %d).\n",
                      dirEntry->d_name, texture->width, texture->height);
     }
    else
     {
      DIR* subDir = opendir(path);
      if(!subDir)
        err(-1, "Couldn't open directory %s in TextureAtlas::TextureAtlas", path);
      processOneAtlas(subDir, path);
      closedir(subDir);
     }
    path[len] = '\0'; // reset path
   }
}


// =======================================================================================
// Constructor for a TA Node.

TANode::TANode(Texture*  T, int width, int height):
              tex(T),
              w(width),
              h(height)
{

}

// =======================================================================================
// Insert a node into the tree we are currently constructing.
// Code loosely based on https://blackpawn.com/texts/lightmaps/default.html

TANode* TANode::insert(Texture* T)
{
/*
    if we're not a leaf then
        (try inserting into first child)
        newNode = child[0]->Insert( img )
        if newNode != NULL return newNode

        (no room, insert into second)
        return child[1]->Insert( img )
    else
        (if there's already a lightmap here, return)
        if imageID != NULL return NULL

        (if we're too small, return)
        if img doesn't fit in pnode->rect
            return NULL

        (if we're just right, accept)
        if img fits perfectly in pnode->rect
            return pnode
        
        (otherwise, gotta split this node and create some kids)
        pnode->child[0] = new Node
        pnode->child[1] = new Node
        
        (decide which way to split)
        dw = rc.width - img.width
        dh = rc.height - img.height
        
        if dw > dh then
            child[0]->rect = Rectangle(rc.left, rc.top,
                                       rc.left+width-1, rc.bottom)
            child[1]->rect = Rectangle(rc.left+width, rc.top,
                                       rc.right, rc.bottom)
        else
            child[0]->rect = Rectangle(rc.left, rc.top,
                                       rc.right, rc.top+height-1)
            child[1]->rect = Rectangle(rc.left, rc.top+height,
                                       rc.right, rc.bottom)
        
        (insert into first child we created)
        return Insert( img, pnode->child[0] )
*/
  return NULL;
}


// =======================================================================================
// Destructor

TextureAtlas::~TextureAtlas(void)
{
}


// =======================================================================================
