// Copyright Staniford Systems.  All Rights Reserved.  Apr 2020 -

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include "Shader.h"

#define TexPathLimit 2048


// =======================================================================================
/// @brief Simple class that provides an abstraction for a single openGL texture.

class Texture
{
 public:
  
  // Instance variables - public
  unsigned        width;                          // these are the internal ones
  unsigned        height;                         // from the image
  int             nrChannels;
  char            textureFileName[TexPathLimit];  // remember where we came from
  GLenum          format;                         // GL_RGBA etc
  unsigned char*  data;                           // holding area for the data
  
  // Member functions - public
  Texture(const char* fileName, bool flip);
  Texture(void);
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


// =======================================================================================

#endif



