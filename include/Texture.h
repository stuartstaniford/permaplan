// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that provides an abstraction for openGL textures.

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Shader.h"

// =======================================================================================
// Class variable initialization


class Texture
{
 public:
  
  // Instance variables - public
  int       width;    // these are the internal ones from the image
  int       height;
  int       nrChannels;

  // Member functions - public
  Texture(const char* fileName);
  ~Texture(void);
  void bind(Shader& shader, unsigned textureUnit, const char* name);

 private:
  
  // Instance variables - private
  unsigned  textureId;
  
  // Member functions - private
  void get2DWrapTextureFromData(GLenum format, GLenum type, void* data);

};

#endif



