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
#include <vector>
#include "TextureAtlas.h"
#include "Logging.h"
#include "GlobalMacros.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define TexPathLimit 2048


// =======================================================================================
// Constructor

TextureAtlas::TextureAtlas(char* dirName):
                treeRoot(NULL)
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
    createImageTree(subDirName);
   }
  closedir(atlasRoot);
}


// =======================================================================================
// Helper function to see if the file name of an extension might reasonably be an image.

bool fileNameAndExtensionCheck(char* fileName)
{
  if(strncmp(fileName, (char*)"atlas.", 6) == 0) // don't consume our own outputs
    return false;
  
  // Now figure out the extension, and sanity check it.
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
// Compare two TANodes and decide which has the shortest perimeter, we want those at the
// beginning of the list (which we will read from the back)

bool perimeterCompare (TANode* t, TANode* u)
{
  return (t->tex->width + t->tex->height < u->tex->width + u->tex->height);
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
    if(dirEntry->d_type == DT_REG && !fileNameAndExtensionCheck(dirEntry->d_name))
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
      TANode* node = new TANode(texture);
      nodeList.push_back(node);
      LogTextureAtlas("Found %s for texture atlas (width %d, height %d, nrChannels %d).\n",
                      dirEntry->d_name, texture->width, texture->height, texture->nrChannels);
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
// Create the image from the TANode tree and write it out.

bool TextureAtlas::saveAtlasImage(char* name)
{
  bool retVal = false;
  unsigned char* buf = new unsigned char[4*width*height];
  for(int i=0; i<4*width*height; i++)
    buf[i] = '\0';
  
  // Put the tree of images into the buffer
  treeRoot->insertIntoImageRecursively(buf, width, height);
  
  // Write the buffer out to the file
  //strncat(name, (char*)"/atlas.png", TexPathLimit);
  //retVal = (bool)stbi_write_png(name, width, height, 4, buf, 4*width);
  strncat(name, (char*)"/atlas.jpg", TexPathLimit);
  retVal = (bool)stbi_write_jpg(name, width, height, 4, buf, 50);

  delete[] buf; buf = NULL;
  return retVal;
}


// =======================================================================================
// After extracting the list of textures from one subdirectory tree, create the in-memory
// image tree required to turn them into a single new texture.

void TextureAtlas::createImageTree(char* name)
{
  width = 0;
  height = 0;
  
  std::sort(nodeList.begin(), nodeList.end(), perimeterCompare);

  // delete any old treeRoot, and set up a bud for the new tree
  if(treeRoot)
   {
    delete treeRoot;
    treeRoot = NULL;
   }
  
  treeRoot = new TANode();
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint *)&(treeRoot->w));
  treeRoot->h = treeRoot->w;
  treeRoot->top = treeRoot->left = 0u;

  // Go through all the textures, adding them to the tree
  while(nodeList.size() > 0)
   {
    TANode* node = nodeList.back();
    nodeList.pop_back(); // node should now be the only pointer to this object
    if(!treeRoot->insert(node, width, height))
       LogTextureAtlas("Texture atlas creation failed at %s (width %d, height %d).\n",
                      node->tex->textureFileName, node->tex->width, node->tex->height);
   }
  
  if(saveAtlasImage(name))
    LogTextureAtlas("Texture atlas creation for %s succeeded (width %d, height %d).\n",
                                        name, width, height);
  else
    LogTextureAtlas("Couldn't create texture atlas file for %s (width %d, height %d).\n",
                                        name, width, height);
}


// =======================================================================================
// Constructor for a TA Node.

TANode::TANode(Texture* T):
              tex(T)
{
  child[0] = NULL;
  child[1] = NULL;
}


// =======================================================================================
// Destructor for a TA Node.

TANode::~TANode(void)
{
  delete child[0];
  delete child[1];
  // note that we don't delete tex, since we don't own it.
}


// =======================================================================================
// Insert a node into the tree we are currently constructing.
// Code loosely based on https://blackpawn.com/texts/lightmaps/default.html

TANode* TANode::insert(TANode* T, unsigned& wd, unsigned& ht)
{
  TANode* retPtr = NULL;
  
  // If there's already a texture here, we are a leaf, return
  if(tex)
    return NULL;

  // We're not a leaf, so try inserting into first child
  if(child[0])
   {
    retPtr = child[0]->insert(T, wd, ht);
    if(retPtr)
      return retPtr;
   }
  
  // No room, try inserting into second
  if(child[1])
   {
    retPtr = child[1]->insert(T, wd, ht);
    if(retPtr)
      return retPtr;
    else
      return NULL;
   }
  else
   {
    // Children don't exist yet, but nor are we a leaf

    // If we're too small, return
    if(T->tex->width > w || T->tex->height > h)
      return NULL;

    // if we're just right, accept
    if(T->tex->width == w && T->tex->height == h)
     {
      // Note this is the place in the code in which a new image is actually entered
      // into the tree.
      tex = T->tex;
      delete T; T=NULL;
      if(left + tex->width > wd)
        wd = left + tex->width;
      if(top + tex->height > ht)
        ht = top + tex->height;
       LogAtlasPlacement("Placing %s (width %d, height %d) at row: %d, col: %d.\n",
                      tex->textureFileName, tex->width, tex->height,
                      top, left);
      return this;
     }
    
    // Otherwise, gotta split this node and create some kids
    child[0] = new TANode;
    child[1] = new TANode;
    
    // Decide which way to split.
    int dw = w - T->tex->width;
    int dh = h - T->tex->height;
        
    if(dw > dh) // left over width greater than left over height
     {
      // split horizontally (around a vertical line)
      child[0]->h     = h;
      child[0]->w     = T->tex->width;
      child[0]->top   = top;
      child[0]->left  = left;
      child[1]->h     = h;
      child[1]->w     = dw;
      child[1]->top   = top;
      child[1]->left  = left + T->tex->width;
     }
    else
     {
      // split vertically (around a horizontal line)
      child[0]->h     = T->tex->height;
      child[0]->w     = w;
      child[0]->top   = top;
      child[0]->left  = left;
      child[1]->h     = dh;
      child[1]->w     = w;
      child[1]->top   = top + T->tex->height;
      child[1]->left  = left;
     }
    // Insert into first child we created
    return child[0]->insert(T, wd, ht);
   }
}


// =======================================================================================
// Destructor

TextureAtlas::~TextureAtlas(void)
{
}


// =======================================================================================
// Recursive function to go down tree and call insertIntoImage on all the leaves.

void TANode::insertIntoImageRecursively(unsigned char* buf, unsigned width, unsigned height)
{
  if(tex)
    insertIntoImage(buf, width, height);
  else
   {
    if(child[0])
     {
      child[0]->insertIntoImageRecursively(buf, width, height);
      child[1]->insertIntoImageRecursively(buf, width, height);
     }
   }
}


// =======================================================================================
// Write a specific image into the image buffer for the Atlas.

void TANode::insertIntoImage(unsigned char* buf, unsigned width, unsigned height)
{
  //fprintf(stderr, "Putting %s into image at %d,%d\n", tex->textureFileName, top, left);
  for(int i=0; i<tex->height; i++)
    for(int j=0; j<tex->width; j++)
     {
       for(int m=0; m<tex->nrChannels; m++)
         buf[((i+top)*width + j + left)*4+m] = tex->data[(i*(tex->width)+j)*tex->nrChannels+m];
       for(int m=tex->nrChannels; m<4; m++)
         buf[((i+top)*width + j + left)*4+m] = '\0';
     }
}


// =======================================================================================
