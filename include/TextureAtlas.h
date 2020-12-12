// Copyright Staniford Systems.  All Rights Reserved.  Oct 2020 -
// Class that provides functionality to load and manage a collection
// of textures into a single texture, and provide appropriate indexing
// and remapping of u,v coords into the subspace within the larger
// texture.  We operate on a directory with sub-directories.  Each
// subdirectory will become a separate atlas, with all the files in
// that subdirectory stored in that particular texture atlas.  The
// end result is a hash table of each "dir/file" as well as the
// atlasses themselves.

#ifndef TEXTURE_ATLAS_H
#define TEXTURE_ATLAS_H

#include <dirent.h>
#include "Texture.h"

// =======================================================================================
// Class variable initialization


// =======================================================================================
// Helper class that is used to hold additional information about a given texture,
// temporarily, while we are in the process of building the Atlas.

class TANode
{
  friend class TextureAtlas;
  friend bool perimeterCompare (TANode* t, TANode* u);

  // Member functions - public
  TANode(Texture*  T = NULL);
  ~TANode(void);
  TANode* insert(TANode* T, unsigned& wd, unsigned& ht);

  private:
    TANode*   child[2];
    Texture*  tex;
    unsigned  top;
    unsigned  left;
    unsigned  w;
    unsigned  h;
};


// =======================================================================================
// Main Atlas class with the code for processing the texture directory
// tree and building that Atlas.

class TextureAtlas
{
 public:
  
  // Instance variables - public

  // Member functions - public
  TextureAtlas(char* dirName);
  ~TextureAtlas(void);
  void processOneAtlas(DIR* dir, char* path);
  void createImageTree(char* name);
  bool saveAtlasImage(char* name, unsigned width, unsigned height);
  

 private:
  
  // Instance variables - private
  TANode* treeRoot;
  std::vector<TANode*> nodeList;
  unsigned width;
  unsigned height;
  
  // Member functions - private
  TextureAtlas(const TextureAtlas&);                 // Prevent copy-construction
  TextureAtlas& operator=(const TextureAtlas&);      // Prevent assignment

};

#endif



