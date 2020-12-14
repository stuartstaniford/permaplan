// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -
// Class that provides an abstraction for openGL textures.

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include "Shader.h"

// =======================================================================================
// Class variable initialization


class Texture
{
 public:
  
  // Instance variables - public
  int             width;            // these are the internal ones
  int             height;           // from the image
  int             nrChannels;
  char*           textureFileName;  // remember where we came from
  GLenum          format;           // GL_RGBA etc
  unsigned char*  data;             // holding area for the data
  
  // Member functions - public
  Texture(const char* fileName);
  ~Texture(void);
  void bind(unsigned textureUnit, const char* name);
  void sendToGpu(void);

 private:
  
  // Instance variables - private
  unsigned  textureId;
  
  // Member functions - private
  Texture(const Texture&);                 // Prevent copy-construction
  Texture& operator=(const Texture&);      // Prevent assignment
};

#endif



